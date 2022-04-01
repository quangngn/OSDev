#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "syscall.h"

#define PROT_NONE 0
#define PROT_EXEC 0x1
#define PROT_WRITE 0x2
#define PROT_READ 0x4

/**
 * Map chunk of virtual memory address starting at vaddr to the address space.
 * For now, we ignore flags, fd, and offset.
 */
void *mmap(void *vaddr, size_t length, int prot, int flags, int fd,
           size_t offset);
/**
 * Unmmap chunk of virtual memory address starting at vaddr.
 */
int munmap(void *vaddr, size_t length);
/**
 * Change the protection of the chunk of virtual memory address starting at
 * vaddr.
 */
int mprotect(void *vaddr, size_t len, int prot);
