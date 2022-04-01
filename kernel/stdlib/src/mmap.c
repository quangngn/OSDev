#include "mmap.h"

/**
 * Map chunk of virtual memory address starting at vaddr to the address space.
 * For now, we ignore flags, fd, and offset.
 */
void *mmap(void *addr, size_t length, int prot, int flags, int fd,
           size_t offset) {
  bool writable = (prot & PROT_WRITE) != 0;
  bool readable = (prot & PROT_READ) != 0;
  bool executable = (prot & PROT_EXEC) != 0;

  uintptr_t cursor = (uintptr_t)addr & PAGE_ALIGN_MASK;
  uintptr_t end = (uintptr_t)addr + length;

  while (cursor < end) {
    if (!syscall(SYSCALL_MMAP, cursor, readable, writable, executable)) {
      return (void *)-1;
    }
    cursor += PAGE_SIZE;
  }
  return addr;
}

/**
 * Unmmap chunk of virtual memory address starting at vaddr.
 */
int munmap(void *addr, size_t length) {
  uintptr_t cursor = (uintptr_t)addr & PAGE_ALIGN_MASK;
  uintptr_t end = (uintptr_t)addr + length;

  while (cursor < end) {
    if (!syscall(SYSCALL_MUNMAP, cursor)) {
      return -1;
    }
    cursor += PAGE_SIZE;
  }
  return 0;
}

/**
 * Change the protection of the chunk of virtual memory address starting at
 * vaddr.
 */
int mprotect(void *addr, size_t length, int prot) {
  bool writable = (prot & PROT_WRITE) != 0;
  bool readable = (prot & PROT_READ) != 0;
  bool executable = (prot & PROT_EXEC) != 0;

  uintptr_t cursor = (uintptr_t)addr & PAGE_ALIGN_MASK;
  uintptr_t end = (uintptr_t)addr + length;

  while (cursor < end) {
    if (!syscall(SYSCALL_MPROTECT, cursor, readable, writable, executable)) {
      return -1;
    }
    cursor += PAGE_SIZE;
  }
  return 0;
}
