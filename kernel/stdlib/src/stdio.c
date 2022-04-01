#include "stdio.h"

// Buffers being used to store digit characters when printing number
char buffer_dec_uint64[NUM_DIGIT_DEC_UINT64 + 1];
char buffer_hex_uint64[NUM_DIGIT_HEX_UINT64 + 1];

// Syscall wrappers: function that invoke the syscall() functions
// Wrapper to call read system call
int64_t sys_read(uint64_t f_descriptor, char* buff, size_t read_size) {
  return syscall(SYSCALL_READ, f_descriptor, (uint64_t)buff, read_size);
}

// Wrapper to call write system call
int64_t sys_write(uint64_t f_descriptor, const char* str, size_t write_size) {
  return syscall(SYSCALL_WRITE, f_descriptor, (uint64_t)str, write_size);
}

/******************************************************************************/
// Print a single character to the terminal
void print_c(char c) { write(STD_OUT, &c, 1); }

// Print a string to the terminal
void print_s(const char* str) {
  write(STD_OUT, str, strlen(str));
}

// Print an unsigned 64-bit integer value to the terminal in decimal notation
// (no leading zeros please!)
void print_d(uint64_t value) {
  if (value == 0) {
    print_c('0');
  } else {
    uint64_t remain = 0;
    size_t num_digit = 0;

    // using the global buffer to store the stringified decimal value
    buffer_dec_uint64[NUM_DIGIT_DEC_UINT64] = '\0';
    char* cursor = buffer_dec_uint64 + NUM_DIGIT_DEC_UINT64;

    // Fill in the buffer from the end to the beginning with each digit of value
    // (from right to left)
    while (value != 0) {
      remain = value % 10;
      value = value / 10;

      *(--cursor) = remain + '0';
      num_digit++;
    }

    write(STD_OUT, cursor, num_digit);
  }
}

// Print an unsigned 64-bit integer value to the terminal in lowercase
// hexadecimal notation (no leading zeros or “0x” please!)
void print_x(uint64_t value) {
  if (value == 0) {
    print_c('0');
  } else {
    uint64_t remain = 0;
    size_t num_digit = 0;

    // using the global buffer to store the stringified hex value
    buffer_hex_uint64[NUM_DIGIT_HEX_UINT64] = '\0';
    char* cursor = buffer_hex_uint64 + NUM_DIGIT_HEX_UINT64;

    // Fill in the buffer from the end to the beginning with each digit of value
    // (from right to left)
    while (value != 0) {
      remain = value % 16;
      value = value / 16;

      // Decrease cursor by 1, set character based on the hex form of remain
      *(--cursor) = remain < 10 ? '0' + remain : 'a' - 10 + remain;
      num_digit++;
    }

    write(STD_OUT, cursor, num_digit);
  }
}

// Print the value of a pointer to the terminal in lowercase hexadecimal with
// the prefix “0x”
void print_p(void* ptr) {
  print_s("0x");
  print_x((uint64_t)ptr);
}

// Print formatted string, supporting:
// - %s for string
// - %c for character
// - %d for decimal number
// - %x for hex number
// - %p for pointer address
int printf(const char* format, ...) {
    const char* cursor = format;
  // Set up va_list to read arguments
  va_list args;
  va_start(args, format);

  // Read each character in format and handle each case when we see %
  while (*cursor != '\0') {
    if (*cursor == '%') {
      // Process the character right after the '%'
      cursor++;
      switch (*cursor) {
        // case "%%" --> print '%'
        case '%':
          print_c('%');
          break;
        // case "%c" --> print next arg as a character
        case 'c':
          print_c(va_arg(args, int));
          break;
        // case "%s" --> print next arg as a string
        case 's':
          print_s(va_arg(args, char*));
          break;
        // case "%d" --> print next arg as a decimal number
        case 'd':
          print_d(va_arg(args, uint64_t));
          break;
        // case "%x" --> print next arg as a hex number
        case 'x':
          print_x(va_arg(args, uint64_t));
          break;
        // case "%p" --> print next arg as a pointer address
        case 'p':
          print_p(va_arg(args, void*));
          break;
        // case "%" -> print nothing, return
        case '\0':
          return;
        // unsupported escape character
        default:
          print_s("<not supported>");
      }
    } else {
      print_c(*cursor);
    }
    cursor++;
  }
}

/******************************************************************************/
/**
 * Read the entire line from stream. The buffer is malloc, null-terminated, and
 * include new line character if it read one. If the buffer is not large enough,
 * then it would be realloc.
 *
 * The function update str and size if the read is correct. If the read is
 * successful, the function returns the number of read characters (excluding the
 * null terminate). Else return -1.
 */
size_t getline(char** str, size_t* size, int* stream) {
  // We need to implement malloc
  return 0;
}

/**
 * Print error
 */
void perror(const char* str) {
  write(STD_ERR, str, strlen(str));
}