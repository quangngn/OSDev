#include "page.h"

extern struct stivale2_struct_tag_memmap* mmap_struct_tag;

/**
 * Translate a virtual address to its mapped physical address
 *
 * \param address The virtual address to translate
 */
void translate(uintptr_t page_table, void* address) {
  // Print address to be translated
  kprintf("Translating %p", address);

  // Get the based address given by hhdm. In this case we assume that it is in
  // section 0
  uint64_t base_viraddr = mmap_struct_tag->memmap[0].base;

  // Get CR3 value
  // Since we cannot cast directly from uint64_t to REG_CR3_CR4_PCIDE0_t, we
  // have to temporarily store the value in temp_cr3. After that we can use
  // pointer cast to treat temp_cr3 as REG_CR3_CR4_PCIDE0_t.
  uint64_t temp_cr3 = (uint64_t)read_cr3();

  REG_CR3_CR4_PCIDE0_t* cr3 = (REG_CR3_CR4_PCIDE0_t*)&temp_cr3;

  // Get address to the start of Page Map Level 4
  // Base dddress of pml4 = (cr3->pml4_phyaddr) + base_viraddr;
  pml4_entry_t* pml4 = (uint64_t)cr3->pml4_phyaddr + base_viraddr;
  pml4 += get_incl_bit(address, 39, 47);

  // Get address to the Page Dir Pointer Table
  pdpt_entry_t* pdpt = (uint64_t)pml4->pdpt_phyaddr + base_viraddr;
  pdpt += get_incl_bit(address, 30, 38);

  // Get address to the Page Dir
  pd_entry_t* pd = (uint64_t)pdpt->pd_phyaddr + base_viraddr;
  pd += get_incl_bit(address, 21, 29);

  // Get address to the page table
  pt_4kb_entry_t* pt = (uint64_t)pd->pt_phyaddr + base_viraddr;
  pt += get_incl_bit(address, 12, 20);

  // Physical Address
  uint64_t phyaddr =
      ((uint64_t)pt->phyaddr << 12) + get_incl_bit(address, 0, 11);
}