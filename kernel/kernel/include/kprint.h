#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include "circular_queue.h"
#include "stivale2.h"

#define NUM_DIGIT_DEC_UINT64 20
#define NUM_DIGIT_HEX_UINT64 16

// Type of function to print to stivale2's terminal
typedef void (*term_write_t)(const char*, size_t);

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

// Set function to term_write
void kset_term_write(term_write_t fn);
// Set value to mmap_struct_tag and hhdm_struct_tag
void kset_mem_struct_tags(
    struct stivale2_struct_tag_memmap* new_mmap_struct_tag,
    struct stivale2_struct_tag_hhdm* new_hhdm_struct_tag);

/**
 * Read one character from the keyboard buffer. If the keyboard buffer is empty
 * this function will block until a key is pressed.
 *
 * \returns the next character input from the keyboard
 */
char kgetc();
