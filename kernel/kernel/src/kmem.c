#include "kmem.h"

// External functions for system call handler. syscall(uint64_t nr, ...) is
// defined in asm/syscall.s
extern int64_t syscall(uint64_t nr, ...);

// Pick a initial virtual memory address to map the heap
static uintptr_t k_heap = KERNEL_HEAP;

// Buffer for distributing memory in malloc. The idea is we first map memory to
// kmbuffer then use it for malloc.
static void* kmbuffer = NULL;
static size_t kremain_mbuffer_size = 0;

/**
 * Invoke system call to map a chunk of memory, starting at vaddr.
 * If vaddr == NULL, the OS choose the next virtual address available in kernel
 * heap.
 * \param vaddr The virtual memory start address to be mapped.
 * \param length Byte size of the memory chunk.
 * \param prot Protection (including read, write, execute permission).
 * \param flags Not used in our OS.
 * \param fd Not used in our OS.
 * \param offset Not used in our OS.
 * \returns the mapped virtual address.
 */
void* kmmap(void* addr, size_t length, int prot, int flags, int fd,
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
    // There isn't any input clue for the virtual address. We choose from kernel
    // heap. Set cursor to the current kernel heap.
    cursor = k_heap;
    // Advance heap pointer to the next page aligned
    k_heap = ROUND_UP(k_heap + length, PAGE_SIZE);
    end = k_heap;
    ret_addr = (void*)cursor;
  }

  // Read proot value
  uintptr_t proot = read_cr3() & PAGE_ALIGN_MASK;
  // Map new pages until we reach sufficient amount of requested memory
  while (cursor < end) {
    if (!vm_map(proot, cursor, readable, writable, executable)) {
      return NULL;
    }
    cursor += PAGE_SIZE;
  }

  // Return the mapped virtual address
  return ret_addr;
}

/**
 * src:
 * https://curtsinger.cs.grinnell.edu/teaching/2022S/CSC395/kernel/libc.html by
 * professor Charlie Curtsinger.
 *
 * Request memory chunk from kernel heap.
 * \param size Size of the memory chunk to be requested.
 * \returns start address of the memory chunk. NULL if the function fails.
 */
void* kmalloc(size_t sz) {
  // Round sz up to a multiple of 16
  sz = ROUND_UP(sz, 16);

  // Do we have enough space to satisfy this allocation?
  if (kremain_mbuffer_size < sz) {
    // No. Get some more space using `mmap`
    size_t rounded_up = ROUND_UP(sz, PAGE_SIZE);
    void* newmem = kmmap(NULL, rounded_up, PROT_READ | PROT_WRITE, 0, -1, 0);
    // Check for errors
    if (newmem == NULL) {
      return NULL;
    }
    kmbuffer = newmem;
    kremain_mbuffer_size = rounded_up;
  }
  // Grab bytes from the beginning of our kmbuffer pointer region
  void* result = kmbuffer;
  kmbuffer = (void*)((uintptr_t)kmbuffer + sz);
  kremain_mbuffer_size -= sz;

  return result;
}

// Free kmalloced memory
void kfree(void* p) { return; }

// Set memory to a certain value
void* kmemset(void* ptr, int value, size_t size) {
  for (int i = 0; i < size; i++) {
    ((unsigned char*)ptr)[i] = (unsigned char)value;
  }
  return ptr;
}

// Copy memory from src to dest. Size is in byte
void kmemcpy(void* dst, void* src, size_t size) {
  if (dst == NULL || src == NULL) return;

  char* d = (char*)dst;
  char* s = (char*)src;

  for (int i = 0; i < size; i++) {
    *d++ = *s++;
  }
}