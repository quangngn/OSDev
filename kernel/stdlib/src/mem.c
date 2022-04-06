#include "mem.h"

// External functions for system call handler. syscall(uint64_t nr, ...) is
// defined in asm/syscall.s
extern int64_t syscall(uint64_t nr, ...);

static uintptr_t user_heap = USER_HEAP;

/**
 * Map chunk of virtual memory address starting at vaddr to the address space.
 * For now, we ignore flags, fd, and offset.
 */
void* mmap(void* addr, size_t length, int prot, int flags, int fd,
           size_t offset) {
  bool writable = (prot & PROT_WRITE) != 0;
  bool readable = (prot & PROT_READ) != 0;
  bool executable = (prot & PROT_EXEC) != 0;

  uintptr_t cursor = 0;
  uintptr_t end = 0;
  if (addr != NULL) {
    cursor = (uintptr_t)addr & PAGE_ALIGN_MASK;
    end = (uintptr_t)addr + length;
  } else {
    // Set cursor to the current heap
    cursor = user_heap;
    // Advance heap pointer to the next page aligned 
    user_heap = ROUND_UP(user_heap + length, PAGE_SIZE);
    end = user_heap;
  }

  while (cursor < end) {
    if (!syscall(SYSCALL_MMAP, cursor, readable, writable, executable)) {
      return NULL;
    }
    cursor += PAGE_SIZE;
  }
  return addr;
}

/**
 * Unmmap chunk of virtual memory address starting at vaddr.
 */
int munmap(void* addr, size_t length) {
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
int mprotect(void* addr, size_t length, int prot) {
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

void* bump = NULL;
size_t space_remaining = 0;

void* malloc(size_t sz) {
  // Round sz up to a multiple of 16
  sz = ROUND_UP(sz, 16);

  // Do we have enough space to satisfy this allocation?
  if (space_remaining < sz) {
    // No. Get some more space using `mmap`
    size_t rounded_up = ROUND_UP(sz, PAGE_SIZE);
    void* newmem = mmap(NULL, rounded_up, PROT_READ | PROT_WRITE, 0, -1, 0);
    // Check for errors
    if (newmem == NULL) {
      return NULL;
    }
    bump = newmem;
    space_remaining = rounded_up;
  }

  // Grab bytes from the beginning of our bump pointer region
  void* result = bump;
  bump += sz;
  space_remaining -= sz;

  return result;
}

void free(void* p) {
  // Do nothing
}

// Copy memory from src to dest. Size is in byte
void memcpy(void* dst, void* src, size_t size) {
  char* d = (char*)dst;
  char* s = (char*)src;

  for (int i = 0; i < size; i++) {
    *d++ = *s++;
  }
}

// Set memory to a certain value
void* memset(void* dst, int value, size_t num) {
  for (int i = 0; i < num; i++) {
    ((unsigned char*)dst)[i] = (unsigned char)value;
  }
  return dst;
}