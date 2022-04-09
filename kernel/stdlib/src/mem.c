#include "mem.h"

#include "stdio.h"

// External functions for system call handler. syscall(uint64_t nr, ...) is
// defined in asm/syscall.s
extern int64_t syscall(uint64_t nr, ...);

// Pick a initial virtual memory address to map the heap
uintptr_t user_heap = USER_HEAP;

// Buffer for distributing memory in malloc. The idea is we first map memory to
// mbuffer then use it for malloc.
void* mbuffer = NULL;
size_t remain_mbuffer_size = 0;

void memtest() {
  printf("[MEM]: %p - mbuffer = %p\n", &mbuffer, mbuffer);
  printf("[MEM]: %p - remain_mbuffer_size = %d\n", &remain_mbuffer_size,
         remain_mbuffer_size);
  printf("[MEM]: %p - user_heap = %p\n\n", &user_heap, user_heap);
}

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
  // ret_addr would be addr if addr is given, else it would be the top of
  // current heap.
  void* ret_addr = NULL;

  if (addr != NULL) {
    // There is an input clue for virtual address to be mapped
    cursor = (uintptr_t)addr & PAGE_ALIGN_MASK;
    end = (uintptr_t)addr + length;
    ret_addr = addr;
  } else {
    // There isn't any input clue for the virtual address. We choose from user
    // heap. Set cursor to the current user heap.
    cursor = user_heap;
    // Advance heap pointer to the next page aligned
    user_heap = ROUND_UP(user_heap + length, PAGE_SIZE);
    end = user_heap;
    ret_addr = (void*)cursor;
  }

  // Map new pages until we reach sufficient amount of requested memory
  while (cursor < end) {
    if (!(bool)syscall(SYSCALL_MMAP, cursor, readable, writable, executable)) {
      return NULL;
    }
    cursor += PAGE_SIZE;
  }

  // Return the mapped virtual address
  return ret_addr;
}

/**
 * Unmmap chunk of virtual memory address starting at vaddr.
 */
int munmap(void* addr, size_t length) {
  uintptr_t cursor = (uintptr_t)addr & PAGE_ALIGN_MASK;
  uintptr_t end = (uintptr_t)addr + length;

  while (cursor < end) {
    if (!(bool)syscall(SYSCALL_MUNMAP, cursor)) {
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
    if (!(bool)syscall(SYSCALL_MPROTECT, cursor, readable, writable,
                       executable)) {
      return -1;
    }
    cursor += PAGE_SIZE;
  }
  return 0;
}

/******************************************************************************/
/**
 * src:
 * https://curtsinger.cs.grinnell.edu/teaching/2022S/CSC395/kernel/libc.html by
 * professor Charlie Curtsinger.
 */
void* malloc(size_t sz) {
  // Round sz up to a multiple of 16
  sz = ROUND_UP(sz, 16);

  // Do we have enough space to satisfy this allocation?
  if (remain_mbuffer_size < sz) {
    // No. Get some more space using `mmap`
    size_t rounded_up = ROUND_UP(sz, PAGE_SIZE);
    void* newmem = mmap(NULL, rounded_up, PROT_READ | PROT_WRITE, 0, -1, 0);
    // Check for errors
    if (newmem == NULL) {
      return NULL;
    }
    mbuffer = newmem;
    remain_mbuffer_size = rounded_up;
  }

  // Grab bytes from the beginning of our mbuffer pointer region
  void* result = mbuffer;
  mbuffer = (void*)((uintptr_t)mbuffer + sz);
  remain_mbuffer_size -= sz;

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