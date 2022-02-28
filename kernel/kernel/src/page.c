#include "page.h"

// hhdm struct allow us to get the base virtual address
extern struct stivale2_struct_tag_hhdm* hhdm_struct_tag;
// memmap struct tag allows us to find the usable memory regions
extern struct stivale2_struct_tag_memmap* mmap_struct_tag;

// Pointer that point to the head of the page structure
page_4kb_t* vfree_list_header = NULL;

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
bool init_free_list() {
  // If the struct tags are not found, we return false
  if (mmap_struct_tag == NULL || hhdm_struct_tag == NULL) {
    kprint_s("[ERROR] init_free_list: failed to read memory struct tags\n");
    return false;
  } else {
    // Get base virtual address of hhdm
    uintptr_t based_vir_addr = hhdm_struct_tag->addr;

    // Find USABLE memory section
    struct stivale2_mmap_entry* mmap_entry;

    // Loop through each mem map section in mmap struct tag to find usable
    // section
    for (int i = 0; i < mmap_struct_tag->entries; i++) {
      mmap_entry = &(mmap_struct_tag->memmap[i]);
      if (mmap_entry->type == STIVALE2_MMAP_TYPE_USABLE) {
        // Virtual address pointing to the base of the memory section. This is
        // also the address of the first page in the section.
        uintptr_t vbase_section = mmap_entry->base + based_vir_addr;

        // Cursor that go through the usable memory section in page size
        page_4kb_t* vcursor = (page_4kb_t*)vbase_section;

        // Add new page to the free list
        while ((uintptr_t)vcursor < vbase_section + mmap_entry->length) {
          // Store the next node's address in elems[0]
          vcursor->elems[0] = (uint64_t)vfree_list_header;
          vfree_list_header = vcursor;
          vcursor++;
        }
      }
    }
  }
}

/**
 * Allocate a page of physical memory. We get the page on the top of the free
 * list.
 *
 * \returns the physical address of the allocated physical memory or 0 on error.
 */
uintptr_t pmem_alloc() {
  if (vfree_list_header == NULL) {
    return 0;
  } else {
    // Get the virtual addr of the new allocated page
    uintptr_t vret_addr = (uintptr_t)vfree_list_header;
    // Advance the free list header to the next page
    vfree_list_header = (page_4kb_t*)(vfree_list_header->elems[0]);
    // Subtract vret_addr to hhdm base addr to get physical address
    return vret_addr - hhdm_struct_tag->addr;
  }
}

/**
 * Free a page of physical memory. The free list is put back the top of the free
 * list.
 *
 * \param p is the physical address of the page to free, which must be
 * page-aligned.
 */
void pmem_free(uintptr_t p) {
  // Early return of p is NULL
  if (p == 0 || p % PAGE_SIZE != 0) {
    kprint_s(
        "[ERROR] pmem_free: the freed page pointer p is either NULL or not "
        "page aligned!\n");
    return;
  }
  // Treat p as a pointer to the block_4kb_t. Put this block at the top of the
  // free list.
  page_4kb_t* vfree_addr = (page_4kb_t*)(p + hhdm_struct_tag->addr);

  // Update the free list' new header by first set the next of vfree_addr to
  // vfree_list_header
  vfree_addr->elems[0] = (uint64_t)vfree_list_header;
  // Make freed page to be the top of the free list
  vfree_list_header = vfree_addr;
}

/**
 * Map a single page of memory into a virtual address space.
 * \param root The physical address of the top-level page table structure
 * \param vaddress The virtual address to map into the address space, must be
 * page-aligned
 * \param user Should the page be user-accessible?
 * \param writable Should the page be writable?
 * \param executable Should the page be executable?
 * \returns true if the mapping succeeded, or false if there was an error
 */
bool vm_map(uintptr_t proot, uintptr_t vaddress, bool user, bool writable,
            bool executable) {
  // Early exit if root address = 0
  if (proot == 0) {
    kprint_s("[ERROR] vm_map: root is NULL\n");
    return false;
  }
  // Early exit if address is not page aligned
  if (vaddress % PAGE_SIZE != 0) {
    kprint_s("[ERROR] vm_map: address is not page aligned\n");
    return false;
  }

  // Get the based address given by hhdm. In this case we assume that it is in
  // section 0
  uint64_t base_viraddr = hhdm_struct_tag->addr;

  // Make an array of paging structure's entries indices from input address
  uint16_t indices[] = {
      (uint64_t)vaddress & 0xFFF, 
      ((uint64_t)vaddress >> 12) & 0x1FF,
      ((uint64_t)vaddress >> 21) & 0x1FF, 
      ((uint64_t)vaddress >> 30) & 0x1FF,
      ((uint64_t)vaddress >> 39) & 0x1FF};

  // Declare paging structure pointers
  pml4_entry_t* vpml4e;
  pdpt_entry_t* vpdpte;
  pd_entry_t* vpde;
  pt_4kb_entry_t* vpte;

  // Access pml4 entry
  vpml4e = (pml4_entry_t*)(proot + base_viraddr);
  vpml4e += indices[4];

  // Access pdpt entry
  if (vpml4e->present == 0) {
    // Allocate place for new pdpt and set its entries to 0
    vpdpte = (pdpt_entry_t*)(pmem_alloc() + base_viraddr);
    if (vpdpte == NULL) return false;
    vpml4e->pdpt_phyaddr = (uint64_t)vpdpte >> 12;
    for (int i = 0; i < NUM_PT_ENTRIES; i++) ((uint64_t*)vpdpte)[i] = 0;
    vpdpte += indices[3];
    // Update pml4e
    vpml4e->present = 1;
    vpml4e->user_access = 1;
    vpml4e->writable = 1;
    vpml4e->exe_disable = 0;
  } else {
    vpdpte = (pdpt_entry_t*)((vpml4e->pdpt_phyaddr << 12) + base_viraddr);
    vpdpte += indices[3];
  }

  // Access pd entry
  if (vpdpte->present == 0) {
    // Allocate place for new pd and set its entries to 0
    vpde = (pd_entry_t*)(pmem_alloc() + base_viraddr);
    if (vpde == NULL) return false;
    vpdpte->pd_phyaddr = (uint64_t)vpde >> 12;
    for (int i = 0; i < NUM_PT_ENTRIES; i++) ((uint64_t*)vpde)[i] = 0;
    vpde += indices[2];
    // Update pdpte
    vpdpte->present = 1;
    vpdpte->user_access = 1;
    vpdpte->writable = 1;
    vpdpte->exe_disable = 0;
  } else {
    vpde = (pd_entry_t*)((vpdpte->pd_phyaddr << 12) + base_viraddr);
    vpde += indices[2];
  }

  // Access pt entry
  if (vpde->present == 0) {
    // Allocate place for new pt and set its entries to 0
    vpte = (pt_4kb_entry_t*)(pmem_alloc() + base_viraddr);
    if (vpte == NULL) return false;
    vpde->pt_phyaddr = (uint64_t)vpte >> 12;
    for (int i = 0; i < NUM_PT_ENTRIES; i++) ((uint64_t*)vpte)[i] = 0;
    vpte += indices[1];
    // Update pde
    vpde->present = 1;
    vpde->user_access = 1;
    vpde->writable = 1;
    vpde->exe_disable = 0;
  } else {
    vpte = (pt_4kb_entry_t*)((vpde->pt_phyaddr << 12) + base_viraddr);
    vpte += indices[1];
  }

  // Map the page to address space
  if (vpte->present == 0) {
    vpte->user_access = user ? 1 : 0;
    vpte->writable = writable ? 1 : 0;
    vpte->exe_disable = executable ? 0 : 1;

    uintptr_t pnew_page_addr = pmem_alloc();
    vpte->phyaddr = pnew_page_addr >> 12;
    return true;
  } else {
    kprint_s("[vm_map FAILED]Page is already mapped\n");
    return false;
  }
}

/**
 * Unmap the page from the memory address space
 * \param proot The physical address of the top-level page table structure.
 * \param vaddress The virtual address to map into the address space, must be
 * page-aligned.
 */
bool vm_unmap(uintptr_t proot, uintptr_t vaddress) {
  // Early exit if root address = 0
  if (proot == 0) {
    kprint_s("[vm_map FAILED] root is NULL\n");
    return false;
  }
  // Early exit if address is not page aligned
  if (vaddress % PAGE_SIZE != 0) {
    kprint_s("[vm_map FAILED] address is not page aligned\n");
    return false;
  }

  // Get the based address given by hhdm. In this case we assume that it is in
  // section 0
  uint64_t base_viraddr = hhdm_struct_tag->addr;

  // Make an array of paging structure's entries indices from input address
  uint16_t indices[] = {
      (uint64_t)vaddress & 0xFFF, ((uint64_t)vaddress >> 12) & 0x1FF,
      ((uint64_t)vaddress >> 21) & 0x1FF, ((uint64_t)vaddress >> 30) & 0x1FF,
      ((uint64_t)vaddress >> 39) & 0x1FF};

  // Declare paging structure pointers
  pml4_entry_t* pml4e;
  pdpt_entry_t* pdpte;
  pdpt_entry_t* pdpt;
  pd_entry_t* pde;
  pd_entry_t* pd;
  pt_4kb_entry_t* pte;
  pt_4kb_entry_t* pt;

  // Access pml4
  pml4e = (pml4_entry_t*)((proot << 12) + (indices[4] << 3) + base_viraddr);

  // Access pdpt
  if (pml4e->present == 1) {
    pdpt = (pdpt_entry_t*)((pml4e->pdpt_phyaddr << 12) + base_viraddr);
    pdpte = pdpt + indices[3];
  } else {
    return true;
  }

  // Access pd
  if (pdpte->present == 1) {
    pd = (pd_entry_t*)((pdpte->pd_phyaddr << 12) + base_viraddr);
    pde = pd + indices[2];
  } else {
    return true;
  }

  // Access pt
  if (pde->present == 1) {
    pt = (pt_4kb_entry_t*)((pd->pt_phyaddr << 12) + base_viraddr);
    pte = pt + indices[1];
  } else {
    return true;
  }

  // Unmap page
  pmem_free(pte->phyaddr << 12);
  pte->present = 0;
  // Check if all pt entries are not present, if so free the higher level table
  // entry
  for (int i = 0; i < NUM_PT_ENTRIES; i++) {
    if (pt[i].present == 1) return true;
  }
  pde->present = 0;

  // Check if all pd entries are not present, if so free the higher level table
  // entry
  for (int i = 0; i < NUM_PT_ENTRIES; i++) {
    if (pd[i].present == 1) return true;
  }
  pdpte->present = 0;

  // Check if all pdpt entries are not present, if so free the higher level
  // table entry
  for (int i = 0; i < NUM_PT_ENTRIES; i++) {
    if (pdpt[i].present == 1) return true;
  }
  pml4e->present = 0;
  return true;
}

/**
 * Translate a virtual address to its mapped physical address
 *
 * \param vaddress The virtual address to translate
 */
void translate(void* vaddress) {
  // Print address to be translated
  kprintf("Translating %p\n", vaddress);

  // Get the based address given by hhdm. In this case we assume that it is in
  // section 0
  uint64_t base_viraddr = hhdm_struct_tag->addr;

  // Make an array of paging structure's entries indices from input address
  uint16_t indices[] = {
      (uint64_t)vaddress & 0xFFF, ((uint64_t)vaddress >> 12) & 0x1FF,
      ((uint64_t)vaddress >> 21) & 0x1FF, ((uint64_t)vaddress >> 30) & 0x1FF,
      ((uint64_t)vaddress >> 39) & 0x1FF};

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
  pml4_entry_t* vpml4 = (pml4_entry_t*)((cr3->pml4_phyaddr << 12) +
                                        (indices[4] << 3) + base_viraddr);

  // Get address to the Page Dir Pointer Table entry
  // pdpt[0:2] = 000
  // pdpt[3:11] = address[30:38]
  // pdpt[12:51] = pml4[12:51]
  pdpt_entry_t* vpdpt = (pdpt_entry_t*)((vpml4->pdpt_phyaddr << 12) +
                                        (indices[3] << 3) + base_viraddr);

  // Get address to the Page Dir entry
  // pd[0:2] = 000
  // pd[3:11] = address[21:29]
  // pd[12:51] = pdpt[12:51]
  pd_entry_t* vpd = (pd_entry_t*)((vpdpt->pd_phyaddr << 12) +
                                  (indices[2] << 3) + base_viraddr);

  // Get address to the Page Table
  // pt[0:2] = 000
  // pt[3:11] = address[12:20]
  // pt[12:51] = pd[12:51]
  pt_4kb_entry_t* vpt = (pt_4kb_entry_t*)((vpd->pt_phyaddr << 12) +
                                          (indices[1] << 3) + base_viraddr);
  kprintf("address present %d\n", vpt->present);

  // Physical Address
  // phyaddr[0:11] = address[0:11]
  // phyaddr[12:51] = pt[12:51]
  uint64_t phy_page_num = vpt->phyaddr;
  uint64_t phyaddr = (phy_page_num << 12) + indices[0];

  // Print Level 4
  kprintf("  Level 4 (index %d of %p)\n", indices[4], cr3->pml4_phyaddr << 12);
  if (vpml4->user_access == 1) {
    kprint_s("    User ");
  } else {
    kprint_s("    Kernel ");
  }
  if (vpml4->writable == 1) {
    kprint_s("Writable ");
  }
  if (vpml4->exe_disable == 0) {
    kprint_s("Executable ");
  }
  kprintf(" -> %p\n", vpdpt);

  // Print Level 3
  kprintf("  Level 3 (index %d of %p)\n", indices[3],
          vpml4->pdpt_phyaddr << 12);
  if (vpdpt->user_access == 1) {
    kprint_s("    User ");
  } else {
    kprint_s("    Kernel ");
  }
  if (vpdpt->writable == 1) {
    kprint_s("Writable ");
  }
  if (vpdpt->exe_disable == 0) {
    kprint_s("Executable ");
  }
  kprintf(" -> %p\n", vpd);

  // Print Level 2
  kprintf("  Level 2 (index %d of %p)\n", indices[2], vpdpt->pd_phyaddr << 12);
  if (vpd->user_access == 1) {
    kprint_s("    User ");
  } else {
    kprint_s("    Kernel ");
  }
  if (vpd->writable == 1) {
    kprint_s("Writable ");
  }
  if (vpd->exe_disable == 0) {
    kprint_s("Executable ");
  }
  kprintf(" -> %p\n", vpt);

  // Print Level 1
  kprintf("  Level 1 (index %d of %p)\n", indices[1], vpd->pt_phyaddr << 12);
  if (vpt->present == 0) {
    kprint_s("Not present\n");
    return;
  }
  if (vpt->user_access == 1) {
    kprint_s("    User ");
  } else {
    kprint_s("    Kernel ");
  }
  if (vpt->writable == 1) {
    kprint_s("Writable ");
  }
  if (vpt->exe_disable == 0) {
    kprint_s("Executable ");
  }
  kprintf(" -> %p\n", phy_page_num);

  // Print conclusion
  kprintf("%p maps to %p\n", vaddress, phyaddr);
}