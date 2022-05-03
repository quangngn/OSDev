#include "syscall.h"

// External functions for system call handler. syscall(uint64_t nr, ...) is
// defined in asm/syscall.s
extern int64_t syscall(uint64_t nr, ...);
extern struct stivale2_struct_tag_framebuffer* framebuffer_struct_tag;
extern size_t screen_w;
extern size_t screen_h;
extern uintptr_t buffer_addr;

/**
 * syscall_handler(...) is being called inside syscall_entry(). Notice that
 * syscall_entry() is invoked by the interrupt 80. Based on the value of arg nr,
 * the function would invoke the appropriate system call handlers.
 */
int64_t syscall_handler(uint64_t nr, uint64_t arg0, uint64_t arg1,
                        uint64_t arg2, uint64_t arg3, uint64_t arg4,
                        uint64_t arg5) {
  uintptr_t proot = 0;
  switch (nr) {
    case SYSCALL_READ:
      /**
       * arg0: file descriptor
       * arg1: pointer to buffer
       * arg2: read size
       * arg3: boolean include newline or not
       * arg4: boolean echo input char to terminal or not.
       * arg5: the number of read character so far.
       */
      return read_handler(arg0, (char*)arg1, arg2, (bool)arg3, (bool)arg4,
                          (int64_t)arg5);
    case SYSCALL_WRITE:
      /**
       * arg0: file descriptor
       * arg1: pointer to string to be printed
       * arg2: write size
       */
      return write_handler(arg0, (const char*)arg1, arg2);
    case SYSCALL_MMAP:
      proot = read_cr3() & PAGE_ALIGN_MASK;
      /**
       * arg0: vaddress
       * arg1: user permission (currently same with readable)
       * arg2: write permission
       * arg3: execute permission
       */
      return vm_map(proot, (uintptr_t)arg0, (bool)arg1, (bool)arg2, (bool)arg3);
    case SYSCALL_MPROTECT:
      proot = read_cr3() & PAGE_ALIGN_MASK;
      /**
       * arg0: vaddress
       * arg1: user permission (currently same with readable)
       * arg2: write permission
       * arg3: execute permission
       */
      return vm_protect(proot, (uintptr_t)arg0, (bool)arg1, (bool)arg2,
                        (bool)arg3);
    case SYSCALL_MUNMAP:
      proot = read_cr3() & PAGE_ALIGN_MASK;
      /**
       * arg0: vaddress to be unmapped
       */
      return vm_unmap(proot, (uintptr_t)arg0);
    case SYSCALL_EXEC:
      /**
       * arg0: name of the executable to be exec.
       */
      return exec_handler((const char*)arg0);
    case SYSCALL_EXIT:
      return exit_handler();
    case SYSCALL_GET_FRAMEBUFFER_INFO:
      /**
       * arg0: pointer to the framebuffer_info_t struct.
       */
      return get_framebuffer_info_handler((framebuffer_info_t*)arg0);
    case SYSCALL_FRAMEBUFFER_CPY:
      /**
       * arg0: source address.
       * arg1: x in pixel coordinate of dst buffer. (top-left origin)
       * arg2: y in pixel coordinate of dst buffer. (top-left origin)
       * arg3: source's buffer width in pixel.
       * arg4: source's buffer height in pixel.
       */
      return framebuffer_cpy_handler((pixel_t*)arg0, (int)arg1, (int)arg2,
                                     (int)arg3, (int)arg4, (bool)arg5);
    default:
      return -1;
  }
}

/******************************************************************************/
// Syscall handlers: functions to process system calls
/**
 * Handlers for read system call. Return the number of read characters
 * (excluding the null-terminate AND backspace). The function is not responsible
 * for adding null terminate. If the function reads backspace, it would remove
 * the previously read character from the buffer and decrease the character
 * count by 1.
 *
 * The read stopped when:
 * - Read null terminate character '\0'.
 * - Reach the read_size.
 * - Read a newline character.
 *
 * Note: read_size also take into account null-terminated. Hence, the size of
 * buff should be read_size.
 *
 * \param f_descriptor Currently only support stdin (aka 0).
 * \param buff Buffer to be write the read character.
 * \param read_size The size of buffer.
 * \param incl_newln Boolean to decide whether to include newline character.
 * \param echo_char Boolean to decide whether to printout read character to
 * terminal.
 * \param read_char_counter The number of character has been read to str before
 * this function is called. The function would continue at reading to
 * buff[read_char_counter]. The use case is when we are reading to str, str run
 * out of space and get realloc. read_handler is called again and continue from
 * the end of str using read_char_counter value.
 * \returns the number of read bytes (excluding null-terminated). Return -1 if
 * the read failed.
 */
int64_t read_handler(uint64_t f_descriptor, char* buff, size_t read_size,
                     bool incl_newln, bool echo_char,
                     int64_t read_char_counter) {
  // Exit if reading from unsupported file descriptors
  if (f_descriptor != STD_IN || buff == NULL) {
    return -1;
  } else {
    // Set input text color
    term_set_color(ARGB32_LIGHT_GREEN, ARGB32_BLACK);

    // Count the number of read characters so far.
    // This variable is also used as the index for the next available slot for
    // new character to be read.
    char c = '\0';
    while (read_char_counter < read_size - 1) {
      c = kget_c();
      // If c is delim (in this case new line), break out of loop
      if (c == '\n') {
        // Add newline to buffer
        if (incl_newln) buff[read_char_counter++] = c;
        // Print newline character and exit loop
        if (echo_char) term_putchar(c);
        break;
      }

      // Else, Handle input character
      if (c == ASCII_BACKSPACE) {
        // Delete the last character and decrease the read_char_counter
        read_char_counter = read_char_counter == 0 ? 0 : read_char_counter - 1;
        buff[read_char_counter] = '\0';
      } else {
        // Put the valid character into the buffer
        buff[read_char_counter++] = c;
      }
      if (echo_char) term_putchar(c);
    }
    // Reset the color setting
    term_reset_color();
    // If we finished reading return the number of read characters
    return read_char_counter;
  }
}

/**
 * Handler for write system call. Function prints characters in str up to the
 * write_size amount of characters. If the characters is null terminate, we
 * return immediately. The functions return -1 if error occurs, otherwise, we
 * return the number of written characters (exclude null terminate).
 *
 * \param f_descriptor: currently support stdout and stderr (aka 1 and 2).
 * \param str: buffer to be written out.
 * \param write_size: number of byte to be written out.
 * \returns -1 if error occurs, otherwise, we return the number of written
 * characters (exclude null terminate).
 */
int64_t write_handler(uint64_t f_descriptor, const char* str,
                      size_t write_size) {
  // Exit early if we do not write to the supported file descriptors or NULL str
  if ((f_descriptor != STD_OUT && f_descriptor != STD_ERR) || str == NULL) {
    return -1;
  }

  // Set terminal color based on
  color_t fg = (f_descriptor == STD_ERR) ? ARGB32_RED : ARGB32_WHITE;
  color_t bg = ARGB32_BLACK;
  term_set_color(fg, bg);

  // Repeat printing characters one by one. If we print null terminate, we
  int i = 0;
  while (i < write_size) {
    if (str[i] != '\0') {
      term_putchar(str[i]);
      i++;
    } else {
      term_reset_color();
      return i;
    }
  }
  term_reset_color();
  return i;
}

/******************************************************************************/
/**
 * Handler to invoke the execution of program with name exec_name.
 * \param exe_name Name of the executable to be exec.
 * \returns true if the function is executed successfully, else return falses.
 */
bool exec_handler(const char* exe_name) { return run_exe(exe_name); }

/**
 * Hanlder to exit the current process and invoke shell exec.
 * \returns true if the function is executed successfully, else return falses.
 */
bool exit_handler() { return run_exe("shell"); }

/******************************************************************************/
/**
 * Handler to handler query kernel's framebuffer information. The information is
 * written to a user's framebuffer information struct.
 * \param fb_info Pointer to the user's framebuffer information struct.
 * \returns true if query successfully and false otherwise.
 */
bool get_framebuffer_info_handler(framebuffer_info_t* fb_info) {
  if (framebuffer_struct_tag == NULL) return false;

  // Set framebuffer information struct
  fb_info->framebuffer_width = framebuffer_struct_tag->framebuffer_width;
  fb_info->framebuffer_height = framebuffer_struct_tag->framebuffer_height;

  fb_info->framebuffer_pitch = framebuffer_struct_tag->framebuffer_pitch;
  fb_info->framebuffer_bpp = framebuffer_struct_tag->framebuffer_bpp;
  fb_info->memory_model = framebuffer_struct_tag->memory_model;
  // Red
  fb_info->red_mask_size = framebuffer_struct_tag->red_mask_size;
  fb_info->red_mask_shift = framebuffer_struct_tag->red_mask_shift;
  // Green
  fb_info->green_mask_size = framebuffer_struct_tag->green_mask_size;
  fb_info->green_mask_shift = framebuffer_struct_tag->green_mask_shift;
  // Blue
  fb_info->blue_mask_size = framebuffer_struct_tag->blue_mask_size;
  fb_info->blue_mask_shift = framebuffer_struct_tag->blue_mask_shift;
  return true;
}

/**
 * Copy src buffer to the kernel's framebuffer at coordinate (dst_x, dst_y).
 * This is similar to drawing a window onto the screen. We don't draw the
 * portion of the window outside of the screen.
 *
 * XY-coordinate has the origin on top-left corner.
 *
 * \param src Address of the source buffer.
 * \param dst_x The x coordinate on the destination buffer.
 * \param dst_y The y coordinate on the destination buffer.
 * \param src_w The width of the source buffer.
 * \param src_h The height of the source buffer.
 * \param flip Boolean whether we flip the source buffer.
 * \returns true if copy successfully and false otherwise.
 */
bool framebuffer_cpy_handler(pixel_t* src, int dst_x, int dst_y, int src_w,
                             int src_h, bool flip) {
  // Check if the buffer is available
  if (framebuffer_struct_tag == NULL) return false;

  // Check for out of bound, in this case we do not need to print
  int dst_x_end = dst_x + src_w;
  int dst_y_end = dst_y + src_h;
  if (dst_x >= screen_w || dst_y >= screen_h || dst_x_end <= 0 ||
      dst_y_end <= 0)
    return true;

  // Compute location to start and end printing in src and end:
  int src_x = 0;
  int src_y = 0;
  int src_x_end = src_w;
  int src_y_end = src_h;

  // If printing destination starts out of left edge ...
  if (dst_x < 0) {
    src_x = -dst_x;
    dst_x = 0;
  }

  // If printing destination starts out of top edge ...
  if (dst_y < 0) {
    src_y = -dst_y;
    dst_y = 0;
  }

  // If printing destination ends out of right edge ...
  if (dst_x_end > screen_w) {
    src_x_end = src_w - (dst_x_end - screen_w);
    dst_x_end = screen_w;
  }

  // If printing destination ends out of bottom edge ...
  if (dst_y_end > screen_h) {
    src_y_end = src_h - (dst_y_end - screen_h);
    dst_y_end = screen_h;
  }

  // Check if offset in source is still within bound
  if (src_x >= src_w || src_y >= src_h || src_x_end <= 0 || src_y_end <= 0)
    return true;

  // Print the source buffer depend on whether you would flip it
  if (flip) {
    src_y = src_h - src_y;
    src_y_end = src_h - src_y_end;

    // Compute memory addressess for source and destination
    src += src_w * src_y + src_x;  // the start of first printed line in src
    pixel_t* dst = (pixel_t*)buffer_addr;
    dst += screen_w * dst_y + dst_x;  // the start of first printed line in dst
    size_t copied_row_byte_size = (src_x_end - src_x) * sizeof(pixel_t);

    for (int i = src_y_end; i < src_y; i++) {
      kmemcpy(dst, src, copied_row_byte_size);
      src -= src_w;
      dst += screen_w;
    }
  } else {
    // Compute memory addressess for source and destination
    src += src_w * src_y + src_x;  // the start of first printed line in src
    pixel_t* dst = (pixel_t*)buffer_addr;
    dst += screen_w * dst_y + dst_x;  // the start of first printed line in dst
    size_t copied_row_byte_size = (src_x_end - src_x) * sizeof(pixel_t);

    for (int i = src_y; i < src_y_end; i++) {
      kmemcpy(dst, src, copied_row_byte_size);
      src += src_w;
      dst += screen_w;
    }
  }

  return true;
}
