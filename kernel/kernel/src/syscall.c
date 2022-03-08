#include "syscall.h"

#include "keyboard.h"

// External functions for system call handler. syscall(uint64_t nr, ...) is
// defined in asm/syscall.s
extern int64_t syscall(uint64_t nr, ...);

// syscall_handler(...) is being called inside syscall_entry(). Notice that
// syscall_entry() is invoked by the interrupt 80. Based on the value of arg nr,
// the function would invoke the appropriate system call handlers
int64_t syscall_handler(uint64_t nr, uint64_t arg0, uint64_t arg1,
                        uint64_t arg2, uint64_t arg3, uint64_t arg4,
                        uint64_t arg5) {
  switch (nr) {
    case SYSCALL_READ:
      /**
       * arg0: file descriptor
       * arg1: pointer to buffer
       * arg2: read size
       */
      return read_handler(arg0, (char*)arg1, arg2);
    case SYSCALL_WRITE:
      /**
       * arg0: file descriptor
       * arg1: pointer to string to be printed
       * arg2: write size
       */
      return write_handler(arg0, (const char*)arg1, arg2);
    default:
      return -1;
  }
}

// Syscall handlers: functions to process system calls
/**
 *  Handlers for read system call. Return the number of read characters
 * (excluding the backspace). Reading stops when the user hit enter (this
 * newline character is not added to buffer). The function is not responsible
 * for adding null terminate. If the function failed to read any characters,
 * return -1.
 */
int64_t read_handler(uint64_t f_descriptor, char* buff, size_t read_size) {
  // Exit if reading from unsupported file descriptors
  if (f_descriptor != STD_IN) {
    return -1;
  } else {
    // Count the number of read characters so far. This is also used as the index
    // for the next available slot for new character to be read.
    int read_char_counter = 0;
    char c = '\0';
    while (read_char_counter < read_size) {
      c = kget_c();
      if (c == '\n') {
        // Stop reading uppon getting newline character
        return read_char_counter;
      } else if (c == ASCII_BACKSPACE) {
        // Delete the last character and decrease the read_char_counter
        read_char_counter = read_char_counter == 0 ? 0 : read_char_counter - 1;
        buff[read_char_counter] = '\0';
      } else {
        // Put the valid character into the buffer
        buff[read_char_counter++] = c;
      }
    }
    // If we finished reading, return the number of read characters
    return read_char_counter;
  }
}

/**
 * Handler for write system call. Function prints characters in str up to the
 * write_size amount of characters. If the characters is null terminate, we
 * return immediately. The functions return -1 if error occurs, otherwise, we
 * return the number of written characters (exclude null terminate).
 */
int64_t write_handler(uint64_t f_descriptor, const char* str,
                      size_t write_size) {
  // Exit early if we do not write to the supported file descriptors
  if (f_descriptor != STD_OUT && f_descriptor != STD_ERR) {
    return -1;
  }

  // Repeat printing characters one by one. If we print null terminate, we 
  int i = 0;
  while (i < write_size) {
    if (str[i] != '\0') {
      kprint_c(str[i]);
      i++;
    } else {
      return i;
    }
  }
  return i;
}

// Syscall wrappers: function that invoke the syscall() functions
// Wrapper to call read system call
int64_t read(uint64_t f_descriptor, char* buff, size_t read_size) {
  return syscall(SYSCALL_READ, f_descriptor, (uint64_t)buff, read_size);
}

// Wrapper to call write system call
int64_t write(uint64_t f_descriptor, const char* str, size_t write_size) {
  return syscall(SYSCALL_WRITE, f_descriptor, (uint64_t)str, write_size);
}