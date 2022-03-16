#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include "string.h"

#define NUM_DIGIT_DEC_UINT64 20
#define NUM_DIGIT_HEX_UINT64 16

#define SYSCALL_READ 0
#define SYSCALL_WRITE 1

#define STD_IN 0
#define STD_OUT 1
#define STD_ERR 2

// Syscall wrappers: function that invoke the syscall() functions
int64_t read(uint64_t f_descriptor, char* buff, size_t read_size);
int64_t write(uint64_t f_descriptor, const char* str, size_t write_size);

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
size_t getline(char** str, size_t* size, int* stream);

/**
 * Print error
 */
void perror(const char* str);