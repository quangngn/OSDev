#pragma once

#include <stdint.h>

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

/**
 * Translate a virtual address to its mapped physical address
 *
 * \param address The virtual address to translate
 */
void translate(void* address);