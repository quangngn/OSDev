#include "stdio.h"

// External functions for system call handler. syscall(uint64_t nr, ...) is
// defined in asm/syscall.s
extern int64_t syscall(uint64_t nr, ...);

// Buffers being used to store digit characters when printing number
static char buffer_dec_uint64[NUM_DIGIT_DEC_UINT64 + 1];
static char buffer_hex_uint64[NUM_DIGIT_HEX_UINT64 + 1];

// Syscall wrappers: function that invoke the syscall() functions
// Wrapper to call read system call
int64_t sys_read(uint64_t f_descriptor, char* buff, size_t read_size,
                 bool incl_newln, bool echo_char) {
  return syscall(SYSCALL_READ, f_descriptor, (uint64_t)buff, read_size,
                 incl_newln, echo_char);
}

// Wrapper to call write system call
int64_t sys_write(uint64_t f_descriptor, const char* str, size_t write_size) {
  return syscall(SYSCALL_WRITE, f_descriptor, (uint64_t)str, write_size);
}

/******************************************************************************/
// Print a single character to the terminal
static void print_c(char c) { sys_write(STD_OUT, &c, 1); }

// Print a string to the terminal
static int print_s(const char* str) {
  int len = strlen(str);
  sys_write(STD_OUT, str, len);
  return len;
}

// Print an unsigned 64-bit integer value to the terminal in decimal notation
// (no leading zeros please!)
static int print_d(uint64_t value) {
  if (value == 0) {
    print_c('0');
    return 1;
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

    sys_write(STD_OUT, cursor, num_digit);
    return num_digit;
  }
}

// Print an unsigned 64-bit integer value to the terminal in lowercase
// hexadecimal notation (no leading zeros or “0x” please!)
static int print_x(uint64_t value) {
  if (value == 0) {
    print_c('0');
    return 1;
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

    sys_write(STD_OUT, cursor, num_digit);
    return num_digit;
  }
}

// Print the value of a pointer to the terminal in lowercase hexadecimal with
// the prefix “0x”
static int print_p(void* ptr) { return print_s("0x") + print_x((uint64_t)ptr); }

// Print formatted string, supporting:
// - %s for string
// - %c for character
// - %d for decimal number
// - %x for hex number
// - %p for pointer address
// Return the number of character printed
int printf(const char* format, ...) {
  if (format == NULL) return 0;

  const char* cursor = format;
  // Set up va_list to read arguments
  va_list args;
  va_start(args, format);

  int char_count = 0;
  // Read each character in format and handle each case when we see %
  while (*cursor != '\0') {
    if (*cursor == '%') {
      // Process the character right after the '%'
      cursor++;
      switch (*cursor) {
        // case "%%" --> print '%'
        case '%':
          print_c('%');
          char_count++;
          break;
        // case "%c" --> print next arg as a character
        case 'c':
          print_c(va_arg(args, int));
          char_count++;
          break;
        // case "%s" --> print next arg as a string
        case 's':
          char_count += print_s(va_arg(args, char*));
          break;
        // case "%d" --> print next arg as a decimal number
        case 'd':
          char_count += print_d(va_arg(args, uint64_t));
          break;
        // case "%x" --> print next arg as a hex number
        case 'x':
          char_count += print_x(va_arg(args, uint64_t));
          break;
        // case "%p" --> print next arg as a pointer address
        case 'p':
          char_count += print_p(va_arg(args, void*));
          break;
        // case "%" -> print nothing, return
        case '\0':
          return char_count;
        // unsupported escape character
        default:
          print_s("<not supported>");
          return 0;
      }
    } else {
      print_c(*cursor);
      char_count++;
    }
    cursor++;
  }
  return char_count;
}

/******************************************************************************/
/**
 * Read the entire line from stream. The buffer is malloc, null-terminated, and
 * include new line character if it read one. If the buffer is not large enough,
 * then it would be realloc. *size would be set to the allocated size of *str.
 *
 * The function update str and size if the read is correct. If the read is
 * successful, the function returns the number of read characters (excluding the
 * null terminate but include delimiter such as new line). Else return -1.
 *
 * The read is done when we read a newline character. This character is also
 * included the *str buffer.
 */
int64_t getline(char** str, size_t* size, int* stream) {
  // Check for NULL pointers from params
  if (str == NULL || size == NULL || stream == NULL) {
    return -1;
  }

  // If *str == NULL, *size is supposed to be 0
  if (*str == NULL) {
    *str = (char*)malloc(sizeof(char) * GETLINE_BUFF_SIZE);
    (*str)[0] = '\0';
    *size = GETLINE_BUFF_SIZE;
  }
  // We read to buffer before copying from buffer to str
  char buffer[GETLINE_BUFF_SIZE];
  // Count the total number of readed character (excluding the null-terminate)
  int64_t str_len = 0;

  while (true) {
    // 1. Read to temp_buff from stream. buffer_len store number of readed
    // characters (excluding null-terminate char)
    int64_t buffer_len = sys_read(*stream, buffer, GETLINE_BUFF_SIZE, true, true);
    buffer[buffer_len] = '\0';

    // 2. If we does not read any character, break from the loop
    if (buffer_len == 0) break;

    // 3. If *str does not have enough space to copy from buffer to *str
    if (buffer_len + str_len + 1 > *size) {
      // 3.1 Allocate new string buffer. Copy from old string buffer to new one
      char* new_str = (char*)malloc(sizeof(char) * (*size + GETLINE_BUFF_SIZE));
      strcpy(new_str, *str);

      // 3.2 Update *str and *size. Free the old string buffer.
      char* old_str = *str;
      *str = new_str;
      *size += GETLINE_BUFF_SIZE;
      free(old_str);
    }
    // 4. Copy from buffer to *str and increase the string length
    strcpy(&(*str)[str_len], buffer);
    str_len += buffer_len;

    // 5. If buffer has newline character, we break from the loop
    if (strchr(buffer, '\n') != NULL) break;
  }
  return str_len;
}

/**
 * Read one character from the stdin file descriptor.
 * Return the read character.
 */
char getc(uint64_t f_descriptor) {
  char ret_c = 0;
  sys_read(f_descriptor, &ret_c, 1, true, true);
  return ret_c;
}

/**
 * Read a string from the given file descriptor. The read stop when:
 * - Reach null-terminate character or newline.
 * - Reach read_size.
 *
 * Note: read_size also include null character. Hence the size of buff should be
 * read_size.
 */
char* fgets(char* buff, size_t read_size, uint64_t f_descriptor) {
  // Call read system call (which does not null-terminate the string for us)
  int64_t len = sys_read(f_descriptor, buff, read_size, true, true);
  // Null-terminate buffer
  buff[len] = '\0';
  return buff;
}

/**
 * Print error message str to STD_ERR
 */
void perror(const char* str) {
  if (str == NULL) return;
  sys_write(STD_ERR, str, strlen(str));
}