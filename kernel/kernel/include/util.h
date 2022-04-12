#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "stivale2.h"

// Halt the CPU in an infinite loop
void halt();

// Set memory to a certain value
void* kmemset(void* ptr, int value, size_t num);

// Copy memory from src to dest. Size is in byte
void kmemcpy(void* dst, void* src, size_t size);

/**
 * Function to call string comparison. In case 1 string is the prefix of the
 * other string, the prefix is consider less than the other string.
 */
int kstrcmp(const char* str1, const char* str2);

// Convert physical address to virtual address by adding hhdm base virtual
// address
uintptr_t ptov(uintptr_t paddr);
