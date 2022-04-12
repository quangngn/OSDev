#pragma once

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "mem.h"
#include "string.h"
#include "system.h"

#define NUM_DIGIT_DEC_UINT64 20
#define NUM_DIGIT_HEX_UINT64 16
#define GETLINE_BUFF_SIZE 32

// Syscall wrappers: function that invoke the syscall() functions
int64_t sys_read(uint64_t f_descriptor, char* buff, size_t read_size,
                 bool incl_newln, bool echo_char);
int64_t sys_write(uint64_t f_descriptor, const char* str, size_t write_size);

// Print formatted string, supporting:
// - %s for string
// - %c for character
// - %d for decimal number
// - %x for hex number
// - %p for pointer address
int printf(const char* format, ...);

/**
 * Read the entire line from stream. The buffer is malloc, null-terminated, and
 * include new line character if it read one. If the buffer is not large enough,
 * then it would be realloc.
 *
 * The function update str and size if the read is correct. If the read is
 * successful, the function returns the number of read characters (excluding the
 * null terminate). Else return -1.
 */
int64_t getline(char** str, size_t* size, int* stream);

/**
 * Read one character from the stdin file descriptor.
 * Return the read character.
 */
char getc(uint64_t f_descriptor);

/**
 * Read a string from the given file descriptor. The read stop when:
 * - Reach null-terminate character or newline.
 * - Reach read_size.
 *
 * Note: read_size also include null character. Hence the size of buff should be
 * read_size.
 */
char* fgets(char* buff, size_t read_size, uint64_t f_descriptor);

/**
 * Print error
 */
void perror(const char* str);