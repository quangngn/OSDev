#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <system.h>

#include "port.h"
#include "page.h"

#ifndef PROT_NONE
#define PROT_NONE 0x0
#endif

#ifndef PROT_EXEC
#define PROT_EXEC 0x1
#endif

#ifndef PROT_WRITE
#define PROT_WRITE 0x2
#endif

#ifndef PROT_READ
#define PROT_READ 0x0
#endif

/**
 * src:
 * https://curtsinger.cs.grinnell.edu/teaching/2022S/CSC395/kernel/libc.html by
 * professor Charlie Curtsinger.
 *
 * Request memory chunk from kernel heap.
 * \param size Size of the memory chunk to be requested.
 * \returns start address of the memory chunk. NULL if the function fails.
 */
void* kmalloc(size_t size);

// Free kmalloced memory
void kfree(void* p);

// Set memory to a certain value
void* kmemset(void* ptr, int value, size_t size);

// Copy memory from src to dest. Size is in byte
void kmemcpy(void* dst, void* src, size_t size);
