#pragma once

#include <stdint.h>
#include <system.h>
#include <stdio.h>

#include "kprint.h"
#include "port.h"
#include "stivale2.h"
#include "util.h"

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

typedef struct page_table_entry {
  bool present : 1;
  bool writable : 1;
  bool user : 1;
  bool write_through : 1;
  bool cache_disable : 1;
  bool accessed : 1;
  bool dirty : 1;
  bool page_size : 1;
  uint8_t _unused0 : 4;
  uintptr_t address : 40;
  uint16_t _unused1 : 11;
  bool no_execute : 1;
} __attribute__((packed)) pt_entry_t;

// Define a struct type that can hold 4Kb data.
// This is going to be used as page frame or entry for other page structure.
typedef struct page_4kb {
  uint64_t elems[NUM_PT_ENTRIES];
} page_4kb_t;

/******************************************************************************/
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
 * \returns the physical address of the allocated physical memory or 0 on error.
 */
uintptr_t pmem_alloc();

/**
 * Free a page of physical memory. The free list is put back the top of the free
 * list.
 * \param p The physical address of the page to be freed, which must be
 * page-aligned.
 */
void pmem_free(uintptr_t p);

/**
 * Similar to pmem_free but the input is expected to be virtual memory.
 * \param v The virtual address of the page to be freed.
 */
inline void vmem_free(uintptr_t v);

/******************************************************************************/
/**
 * Map a single page of memory into a virtual address space.
 * \param root The physical address of the top-level page table structure.
 * \param vaddress The virtual address to map into the address space.
 * \param user Boolean for user-accessible (also used for read permission).
 * \param writable Boolean for write permission.
 * \param executable Boolean for execute permission.
 * \returns true if the mapping succeeded, else return false.
 */
bool vm_map(uintptr_t proot, uintptr_t vaddress, bool user, bool writable,
            bool executable);

/**
 * Unmap the page from the memory address space.
 * \param proot The physical address of the top-level page table structure.
 * \param vaddress The virtual address to unmap from the address space.
 * \returns true if unmap successfully, else returns false.
 */
bool vm_unmap(uintptr_t proot, uintptr_t vaddress);

/**
 * Change the protection mode of the mapped page. If the virtual address is not
 * mapped, we return false. Return true if mode change success.
 * \param root The physical address of the top-level page table structure.
 * \param vaddress The virtual address.
 * \param user Boolean for user-accessible (also used for read permission).
 * \param writable Boolean for write permission.
 * \param executable Boolean for execute permission.
 * \returns true if the changing permission succeeded, else return false.
 */
bool vm_protect(uintptr_t proot, uintptr_t vaddress, bool user, bool writable,
                bool executable);

/**
 * By professor Charlie Curtsinger
 * src:
 * https://curtsinger.cs.grinnell.edu/teaching/2022S/CSC395/kernel/housekeeping.html
 *
 * Unmap everything in the lower half of an address space with level 4 page
 * table at address root.
 * \param root The physical address of the top-level page table structure.
 */
void unmap_lower_half(uintptr_t root);

/******************************************************************************/
/**
 * Translate a virtual address to its mapped physical address
 * \param vaddress The virtual address to translate.
 */
void translate(void* vaddress);
