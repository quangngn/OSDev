#include "syscall.h"

// External functions for system call handler. syscall(uint64_t nr, ...) is
// defined in asm/syscall.s
extern int64_t syscall(uint64_t nr, ...);

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
      return exec_handler((const char*) arg0);
    case SYSCALL_EXIT:
      return exit_handler();
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

/**
 * Handler to invoke the execution of program with name exec_name.
 * \param exe_name Name of the executable to be exec.
 * \returns true if the function is executed successfully, else return falses.
 */
bool exec_handler(const char* exe_name) {
  return run_exe(exe_name);
}

/**
 * Hanlder to exit the current process and invoke shell exec.
 * \returns true if the function is executed successfully, else return falses.
 */
bool exit_handler() {
  return run_exe("shell");
}