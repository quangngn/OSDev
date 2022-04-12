#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include "keyboard.h"
#include "stivale2.h"
#include "term.h"

#define NUM_DIGIT_DEC_UINT64 20
#define NUM_DIGIT_HEX_UINT64 16

// Type of function to print to stivale2's terminal
typedef void (*term_write_t)(const char*, size_t, uint8_t fg, uint8_t bg);

// Set function to term_write
void kset_term_write(term_write_t fn);

// Count the number of character in the input string
size_t kstrlen(const char* str);
// Print a single character to the terminal
void kprint_c(char c);
// Print a string to the terminal
void kprint_s(const char* str);
// Print an unsigned 64-bit integer value to the terminal in decimal notation
// (no leading zeros please!)
void kprint_d(uint64_t value);
// Print an unsigned 64-bit integer value to the terminal in lowercase
// hexadecimal notation (no leading zeros or “0x” please!)
void kprint_x(uint64_t value);
// Print the value of a pointer to the terminal in lowercase hexadecimal with
// the prefix “0x”
void kprint_p(void* ptr);
// Print formatted string, supporting:
// - %s for string
// - %c for character
// - %d for decimal number
// - %x for hex number
// - %p for pointer address
void kprintf(const char* format, ...);
// Print usable memory in format:
// "[P. Addr start]-[P. Addr end] mapped at [Vir. Addr start] - [Vir. Addr end]
void kprint_mem_usage();

/*
 * Read one character from the keyboard buffer. If the keyboard buffer is empty
 * this function will block until a key is pressed.
 *
 * \returns the next character input from the keyboard
 */
char kget_c();
/**
 * Read a line of characters from the keyboard. Read characters until the buffer
 * fills or a newline character is read. If input ends with a newline, the
 * newline character is stored in output. The string written to output is always
 * null terminated unless the function fails for some reason.
 *
 * \param output A pointer to the beginning of an array where this function
 * should store characters. This function will write a null terminator into the
 * output array unless it fails. 
 * 
 * \param capacity The number of characters that can safely be written to the
 * output array including the final null char.
 * 
 * \returns The number of characters read, or zero if no characters
 * were read due to an error.
 */
size_t kget_s(char* output, size_t capacity);
