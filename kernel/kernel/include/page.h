#pragma once

#include <stdint.h>

#include "kprint.h"
#include "port.h"
#include "stivale2.h"
#include "util.h"

#define MAGIC_NUM 0x234ab234

#define NUM_PT_ENTRIES 512
// This is also the size of pml4 entry, pdpt entry, pd entry
#define BYTE_SIZE_OF_PT_ENTRY 64
// PAGE_SIZE = NUM_PT_ENTRIES * BYTE_SIZE_OF_PT_ENTRY
#define PAGE_SIZE 4094

// CR3 for Ordinary 4-level mapping with CR4.PCIDE = 0
// Notice that PCIDE is the bit 17 of CR4
typedef struct REG_CR3_CR4_PCIDE0 {
  // bit 0
  uint64_t ignored0 : 3;
  uint64_t page_write_through : 1;
  uint64_t cache_disable : 1;
  uint64_t ignored1 : 7;
  // bit 12:51
  uint64_t pml4_phyaddr : 40;
  // bit 52:63
  uint64_t reserved0 : 12;
} __attribute__((packed)) REG_CR3_CR4_PCIDE0_t;

// CR3 for Ordinary 4-level mapping with PR4.PCIDE = 1
// Notice that PCIDE is the bit 17 of CR4
typedef struct REG_CR3_CR4_PCIDE1 {
  // bit 0
  uint64_t PCID : 12;
  // bit 12:51
  uint64_t pml4_phyaddr : 40;
  // bit 52:63
  uint64_t reserved0 : 12;
} __attribute__((packed)) REG_CR3_CR4_PCIDE1_t;

// Page Map Level 4 entry struct
typedef struct pml4_entry {
  // bit 0
  uint64_t present : 1;
  uint64_t writable : 1;
  uint64_t user_access : 1;
  uint64_t page_write_through : 1;
  uint64_t cache_disable : 1;
  // bit 5
  uint64_t accessed : 1;
  uint64_t ignored0 : 1;
  uint64_t reserved0 : 1;  // 1 if use 2mb page
  uint64_t ignored1 : 3;
  // bit 11
  uint64_t restart : 1;
  // bit 12:51
  uint64_t pdpt_phyaddr : 40;
  // bit 52:62
  uint64_t ignored2 : 11;
  // bit 63
  uint64_t exe_disable : 1;
} __attribute__((packed)) pml4_entry_t;

// Page Dir Pointer Table entry that references Page Dir
typedef struct pdpt_entry {
  // bit 0
  uint64_t present : 1;
  uint64_t writable : 1;
  uint64_t user_access : 1;
  uint64_t page_write_through : 1;
  uint64_t cache_disable : 1;
  // bit 5
  uint64_t accessed : 1;
  uint64_t ignored0 : 1;
  uint64_t page_size : 1;
  uint64_t ignored1 : 3;
  // bit 11
  uint64_t restart : 1;
  // bit 12:51
  uint64_t pd_phyaddr : 40;
  // bit 52:62
  uint64_t ignored2 : 11;
  // bit 63
  uint64_t exe_disable : 1;
} __attribute__((packed)) pdpt_entry_t;

// Page Dir entry that reference Page Table
typedef struct pd_entry {
  // bit 0
  uint64_t present : 1;
  uint64_t writable : 1;
  uint64_t user_access : 1;
  uint64_t page_write_through : 1;
  uint64_t cache_disable : 1;
  // bit 5
  uint64_t accessed : 1;
  uint64_t ignored0 : 1;
  uint64_t page_size : 1;
  uint64_t ignored1 : 3;
  // bit 11
  uint64_t restart : 1;
  // bit 12:51
  uint64_t pt_phyaddr : 40;
  // bit 52:62
  uint64_t ignored2 : 11;
  // bit 63
  uint64_t exe_disable : 1;
} __attribute__((packed)) pd_entry_t;

typedef struct pt_4kb_entry {
  // bit 0
  uint64_t present : 1;
  uint64_t writable : 1;
  uint64_t user_access : 1;
  uint64_t page_write_through : 1;
  uint64_t cache_disable : 1;
  // bit 5
  uint64_t accessed : 1;
  uint64_t dirty : 1;
  uint64_t PAT : 1;
  uint64_t global : 1;
  uint64_t ignored0 : 2;
  // bit 11
  uint64_t restart : 1;
  // bit 12:51
  uint64_t phyaddr : 40;
  // bit 52:58
  uint64_t ignored1 : 7;
  // bit 59:62
  uint64_t proc_key : 4;
  // bit 63
  uint64_t exe_disable : 1;
} __attribute__((packed)) pt_4kb_entry_t;

// Define a struct type that can hold 4Kb data.
// This is going to be used as page frame or entry for other page structure.
typedef struct page_4kb {
  uint64_t elems[NUM_PT_ENTRIES];
} page_4kb_t;

/**
 * Initialized the free list structure in USABLE memory sections. Each block of
 * this list would be 4kb.
 *
 * Each list element is one page_4kb_t element. The address to the next page
 * would be stored in elems[0] (see the struct definition in page.h). The
 * address is virtual address.
 *
 * We imagine that each free memory section is an array of page_4kb_t element.
 * We will loop through them and add them to the top of the free list structure.
 */
bool init_free_list();

/**
 * Allocate a page of physical memory. We get the page on the top of the free
 * list.
 *
 * \returns the physical address of the allocated physical memory or 0 on error.
 */
uintptr_t pmem_alloc();

/**
 * Free a page of physical memory. The free list is put back the top of the free
 * list.
 *
 * \param p is the physical address of the page to free, which must be
 * page-aligned.
 */
void pmem_free(uintptr_t p);

/**
 * Map a single page of memory into a virtual address space.
 * \param proot The physical address of the top-level page table structure
 * \param vaddress The virtual address to map into the address space, must be
 * page-aligned
 * \param user Should the page be user-accessible?
 * \param writable
 * Should the page be writable?
 * \param executable Should the page be executable?
 * \returns true if the mapping succeeded, or false if there was an error
 */
bool vm_map(uintptr_t proot, uintptr_t vaddress, bool user, bool writable,
            bool executable);

/**
 * Unmap a single page of memory from the virtual address space.
 * \param proot The physical address of the top-level page table structure
 * \param vaddress The virtual address to map into the address space, must be
 * page-aligned
 */
bool vm_unmap(uintptr_t proot, uintptr_t vaddress);

/**
 * Translate a virtual address to its mapped physical address
 *
 * \param vaddress The virtual address to translate
 */
void translate(void* vaddress);