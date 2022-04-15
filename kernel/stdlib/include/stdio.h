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
                 bool incl_newln, bool echo_char, int64_t read_char_counter);
int64_t sys_write(uint64_t f_descriptor, const char* str, size_t write_size);

/**
 * Print unformatted string str.
 * \param f_descriptor The file descriptor to be printed to.
 * \param str The string to be printed.
 * \returns number of characters being printed.
 */
int fprint_s(uint64_t f_descriptor, const char* str);

/**
 * Print formatted string, supporting:
 * - %s for string
 * - %c for character
 * - %d for decimal number
 * - %x for hex number
 * - %p for pointer address
 * - %% would be character '%'.
 * \param format The format string.
 */
int printf(const char* format, ...);

/**
 * Print formatted error message, supporting:
 * - %s for string
 * - %c for character
 * - %d for decimal number
 * - %x for hex number
 * - %p for pointer address
 * - %% would be character '%'.
 * \param format The format string.
 */
void perror(const char* format, ...);

/**
 * Read the entire line from stream to str. The *str is malloc, null-terminated,
 * and include new line character if it read one. If the buffer is not large
 * enough, then it would be realloc. size would be update to match the size of
 * the allocated buffer.
 *
 * \param str String buffer to be written to. It would be reallocated if we do
 * not have enough space. size would be update accordingly.
 * \param size pointer to the size of the allocated string buffer.
 * \param stream pointer to the input stream.
 * \returns the number of read characters (excluding the null terminate). Else
 * return -1.
 */
int64_t getline(char** str, size_t* size, uint64_t* stream);

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