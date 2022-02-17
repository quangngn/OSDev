#include "page.h"

extern struct stivale2_struct_tag_memmap* mmap_struct_tag;

/**
 * Translate a virtual address to its mapped physical address
 *
 * \param address The virtual address to translate
 */
void translate(void* address) {
  // Print address to be translated
  kprintf("Translating %p\n", address);

  // Get the based address given by hhdm. In this case we assume that it is in
  // section 0
  uint64_t base_viraddr = mmap_struct_tag->memmap[0].base;

  // Get CR3 value
  // Since we cannot cast directly from uint64_t to REG_CR3_CR4_PCIDE0_t, we
  // have to temporarily store the value in temp_cr3. After that we can use
  // pointer cast to treat temp_cr3 as REG_CR3_CR4_PCIDE0_t.
  uint64_t temp_cr3 = (uint64_t)read_cr3();
  REG_CR3_CR4_PCIDE0_t* cr3 = (REG_CR3_CR4_PCIDE0_t*)(&temp_cr3);

  // Get address to the start of Page Map Level 4
  pml4_entry_t* pml4_base = (pml4_entry_t*)(cr3->pml4_phyaddr + base_viraddr);
  pml4_entry_t* pml4 = pml4_base + get_incl_bit((uint64_t)address, 39, 47);

  // Get address to the Page Dir Pointer Table
  pdpt_entry_t* pdpt_base = (pdpt_entry_t*)(pml4->pdpt_phyaddr + base_viraddr);
  pdpt_entry_t* pdpt = pdpt_base + get_incl_bit((uint64_t)address, 30, 38);

  // Get address to the Page Dir
  pd_entry_t* pd_base = (pd_entry_t*)(pdpt->pd_phyaddr + base_viraddr);
  pd_entry_t* pd = pd_base + get_incl_bit((uint64_t)address, 21, 29);

  // Get address to the page table
  pt_4kb_entry_t* pt_base = (pt_4kb_entry_t*)(pd->pt_phyaddr + base_viraddr);
  pt_4kb_entry_t* pt = pt_base + get_incl_bit((uint64_t)address, 12, 20);

  // Physical Address
  uint64_t phy_page_num = pt->phyaddr << 12;
  uint64_t phyaddr = phy_page_num + get_incl_bit((uint64_t)address, 0, 11);

  // Print Level 4
  kprintf("  Level 4 (index %d of %p)\n",
          get_incl_bit((uint64_t)address, 39, 47), pml4_base);
  if (pml4->user_access == 1) {
    kprint_s("    User ");
  } else {
    kprint_s("    Kernel ");
  }
  if (pml4->writable == 1) {
    kprint_s("Writable ");
  }
  if (pml4->exe_disable == 0) {
    kprint_s("Executable ");
  }
  kprintf(" -> %p\n", pdpt_base);

  // Print Level 3
  kprintf("  Level 3 (index %d of %p)\n",
          get_incl_bit((uint64_t)address, 30, 38), pdpt_base);
  if (pdpt->user_access == 1) {
    kprint_s("    User ");
  } else {
    kprint_s("    Kernel ");
  }
  if (pdpt->writable == 1) {
    kprint_s("Writable ");
  }
  if (pdpt->exe_disable == 0) {
    kprint_s("Executable ");
  }
  kprintf(" -> %p\n", pd_base);

  // Print Level 2
  kprintf("  Level 2 (index %d of %p)\n",
          get_incl_bit((uint64_t)address, 21, 29), pd_base);
  if (pd->user_access == 1) {
    kprint_s("    User ");
  } else {
    kprint_s("    Kernel ");
  }
  if (pd->writable == 1) {
    kprint_s("Writable ");
  }
  if (pd->exe_disable == 0) {
    kprint_s("Executable ");
  }
  kprintf(" -> %p\n", pt_base);

  // Print Level 1
  kprintf("  Level 1 (index %d of %p)\n",
          get_incl_bit((uint64_t)address, 12, 20), pt_base);
  if (pt->present == 0) {
    kprint_s("Not present\n");
    return;
  }
  if (pt->user_access == 1) {
    kprint_s("    User ");
  } else {
    kprint_s("    Kernel ");
  }
  if (pt->writable == 1) {
    kprint_s("Writable ");
  }
  if (pt->exe_disable == 0) {
    kprint_s("Executable ");
  }
  kprintf(" -> %p\n", phy_page_num);

  // Print conclusion
  kprintf("%p maps to %p\n", address, phyaddr);
}