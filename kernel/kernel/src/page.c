#include "page.h"

// hhdm struct allow us to get the base virtual address
extern struct stivale2_struct_tag_hhdm* hhdm_struct_tag;

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
  uint64_t base_viraddr = hhdm_struct_tag->addr;

  // Make an array of paging structure's entries indices from input address
  uint16_t indices[] = {
      (uint64_t)address & 0xFFF, 
      ((uint64_t)address >> 12) & 0x1FF,
      ((uint64_t)address >> 21) & 0x1FF, 
      ((uint64_t)address >> 30) & 0x1FF,
      ((uint64_t)address >> 39) & 0x1FF};

  // Get CR3 value
  // Since we cannot cast directly from uint64_t to REG_CR3_CR4_PCIDE0_t, we
  // have to temporarily store the value in temp_cr3. After that we can use
  // pointer cast to treat temp_cr3 as REG_CR3_CR4_PCIDE0_t.
  uint64_t temp_cr3 = (uint64_t)read_cr3();
  REG_CR3_CR4_PCIDE0_t* cr3 = (REG_CR3_CR4_PCIDE0_t*)(&temp_cr3);

  // Get address to the Page Map Level 4 entry
  // pml4[0:2] = 000
  // pml4[3:11] = address[39:47]
  // pml4[12:51] = cr3[12:51]
  pml4_entry_t* pml4 = (pml4_entry_t*)((cr3->pml4_phyaddr << 12) +
                                       (indices[4] << 3) + base_viraddr);

  // Get address to the Page Dir Pointer Table entry
  // pdpt[0:2] = 000
  // pdpt[3:11] = address[30:38]
  // pdpt[12:51] = pml4[12:51]
  pdpt_entry_t* pdpt = (pdpt_entry_t*)((pml4->pdpt_phyaddr << 12) +
                                       (indices[3] << 3) + base_viraddr);

  // Get address to the Page Dir entry
  // pd[0:2] = 000
  // pd[3:11] = address[21:29]
  // pd[12:51] = pdpt[12:51]
  pd_entry_t* pd = (pd_entry_t*)((pdpt->pd_phyaddr << 12) + (indices[2] << 3) +
                                 base_viraddr);

  // Get address to the Page Table
  // pt[0:2] = 000
  // pt[3:11] = address[12:20]
  // pt[12:51] = pd[12:51]
  pt_4kb_entry_t* pt = (pt_4kb_entry_t*)((pd->pt_phyaddr << 12) +
                                         (indices[1] << 3) + base_viraddr);
  kprintf("address present %d\n", pt->present);

  // Physical Address
  // phyaddr[0:11] = address[0:11]
  // phyaddr[12:51] = pt[12:51] 
  uint64_t phy_page_num = pt->phyaddr;
  uint64_t phyaddr = (phy_page_num << 12) + indices[0];

  // Print Level 4
  kprintf("  Level 4 (index %d of %p)\n", indices[4], cr3->pml4_phyaddr << 12);
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
  kprintf(" -> %p\n", pdpt);

  // Print Level 3
  kprintf("  Level 3 (index %d of %p)\n", indices[3], pml4->pdpt_phyaddr << 12);
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
  kprintf(" -> %p\n", pd);

  // Print Level 2
  kprintf("  Level 2 (index %d of %p)\n", indices[2], pdpt->pd_phyaddr << 12);
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
  kprintf(" -> %p\n", pt);

  // Print Level 1
  kprintf("  Level 1 (index %d of %p)\n", indices[1], pd->pt_phyaddr << 12);
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