#pragma once

#include <stdarg.h>
#include <stdint.h>

#include "stivale2.h"
#include "util.h"

#define NUM_DIGIT_DEC_UINT64 20
#define NUM_DIGIT_HEX_UINT64 16

extern term_write_t term_write;

// Count the number of character in the input string
size_t kstr_length(const char* str);

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
