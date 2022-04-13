#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "system.h"

// Round a value x up to the next multiple of y
#define ROUND_UP(x, y) ((x) % (y) == 0 ? (x) : (x) + ((y) - (x) % (y)))

#define PROT_NONE 0
#define PROT_EXEC 0x1
#define PROT_WRITE 0x2
#define PROT_READ 0x4

/**
 * Invoke system call to map a chunk of memory, starting at vaddr.
 * If vaddr == NULL, the OS choose the next virtual address available in user
 * heap.
 * \param vaddr The virtual memory start address to be mapped.
 * \param length Byte size of the memory chunk.
 * \param prot Protection (including read, write, execute permission).
 * \param flags Not used in our OS.
 * \param fd Not used in our OS.
 * \param offset Not used in our OS.
 * \returns the mapped virtual address.
 */
void* mmap(void* vaddr, size_t length, int prot, int flags, int fd,
           size_t offset);

/**
 * Invoke system call to unmap a chunk of memory, starting at vaddr.
 * \param vaddr The virtual memory start address to be unmapped.
 * \param length Byte size of the memory chunk.
 * \returns 0 if successful, else -1.
 */
int munmap(void* vaddr, size_t length);

/**
 * Change the protection of the chunk of virtual memory address starting at
 * vaddr.
 * \param vaddr The virtual memory start address to be set protection.
 * \param length Byte size of the memory chunk.
 * \param prot Protection (including read, write, execute permission).
 * \returns 0 if successful, else -1.
 */
int mprotect(void* vaddr, size_t len, int prot);

/******************************************************************************/
/**
 * Request memory chunk from heap.
 * \param size Size of the memory chunk to be requested.
 * \returns start address of the memory chunk. NULL if the function fails.
 */
void* malloc(size_t size);

/**
 * Set value to a memory chunk.
 * \param dst Base address of the memory chunk.
 * \param value Value to be set.
 * \param size Size of the memory chunk.
 * \param return pointer to the memory chunk.
 */ 
void* memset(void* dst, int value, size_t size);

/**
 * Copy memory from src to dst.
 * \param dst Base memory address of the destination memory.
 * \param src Base memory address of the source memory.
 * \param size Size of the memory chunk.
 */ 
void memcpy(void* dst, void* src, size_t size);

/**
 * Free memory p
 */ 
void free(void* p);