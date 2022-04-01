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
    kprint_s("[ERROR] init_free_list: Failed to read memory struct tags\n");
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
    return true;
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
 * Similar to pmem_free but the input is expected to be virtual memory.
 */
inline void vmem_free(uintptr_t v) { pmem_free(v - hhdm_struct_tag->addr); }

/******************************************************************************/
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
    kprint_s("[ERROR] vm_map: proot is NULL\n");
    return false;
  }

  // Get the based address given by hhdm. In this case we assume that it is in
  // section 0
  uint64_t base_vaddr = hhdm_struct_tag->addr;

  // Make an array of paging structure's entries indices from input address
  uint16_t indices[] = {
      (uint64_t)vaddress & 0xFFF, ((uint64_t)vaddress >> 12) & 0x1FF,
      ((uint64_t)vaddress >> 21) & 0x1FF, ((uint64_t)vaddress >> 30) & 0x1FF,
      ((uint64_t)vaddress >> 39) & 0x1FF};

  // Declare paging structure pointers
  pml4_entry_t* vpml4e;
  pdpt_entry_t* vpdpte;
  pd_entry_t* vpde;
  pt_4kb_entry_t* vpte;

  // Access pml4 entry
  vpml4e = (pml4_entry_t*)(proot + base_vaddr) + indices[4];

  // Access pdpt entry
  if (vpml4e->present == 0) {
    // Allocate place for new pdpt and set its entries to 0
    // vpdpte is currently pointing to first entry of the pdpt
    vpdpte = (pdpt_entry_t*)pmem_alloc();
    if ((uintptr_t)vpdpte == 0) return false;
    vpml4e->pdpt_phyaddr = (uint64_t)vpdpte >> 12;
    vpdpte = (pdpt_entry_t*)((uintptr_t)vpdpte + base_vaddr);
    for (int i = 0; i < NUM_PT_ENTRIES; i++) ((uint64_t*)vpdpte)[i] = 0;
    // vpdpte is currently pointing to entry leading to target page table
    vpdpte += indices[3];
    // Update pml4e
    vpml4e->present = 1;
    vpml4e->user_access = 1;
    vpml4e->writable = 1;
    vpml4e->exe_disable = 0;
  } else {
    vpdpte =
        (pdpt_entry_t*)((vpml4e->pdpt_phyaddr << 12) + base_vaddr) + indices[3];
  }

  // Access pd entry
  if (vpdpte->present == 0) {
    // Allocate place for new pd and set its entries to 0
    // vpde is currently pointing to first entry of the pd
    vpde = (pd_entry_t*)pmem_alloc();
    if ((uintptr_t)vpde == 0) return false;
    vpdpte->pd_phyaddr = (uint64_t)vpde >> 12;
    vpde = (pd_entry_t*)((uintptr_t)vpde + base_vaddr);
    for (int i = 0; i < NUM_PT_ENTRIES; i++) ((uint64_t*)vpde)[i] = 0;
    // vpde is currently pointing to entry leading to target page table
    vpde += indices[2];
    // Update pdpte
    vpdpte->present = 1;
    vpdpte->user_access = 1;
    vpdpte->writable = 1;
    vpdpte->exe_disable = 0;
  } else {
    vpde = (pd_entry_t*)((vpdpte->pd_phyaddr << 12) + base_vaddr) + indices[2];
  }

  // Access pt entry
  if (vpde->present == 0) {
    // Allocate place for new pt and set its entries to 0
    // vpte is currently pointing to first entry of the pd
    vpte = (pt_4kb_entry_t*)pmem_alloc();
    if ((uintptr_t)vpte == 0) return false;
    vpde->pt_phyaddr = (uint64_t)vpte >> 12;
    vpte = (pt_4kb_entry_t*)((uintptr_t)vpte + base_vaddr);
    for (int i = 0; i < NUM_PT_ENTRIES; i++) ((uint64_t*)vpte)[i] = 0;
    // vpte is currently pointing to of target page table entry
    vpte += indices[1];
    // Update pde
    vpde->present = 1;
    vpde->user_access = 1;
    vpde->writable = 1;
    vpde->exe_disable = 0;
  } else {
    vpte =
        (pt_4kb_entry_t*)((vpde->pt_phyaddr << 12) + base_vaddr) + indices[1];
  }

  // Map the page to address space
  if (vpte->present == 0) {
    uintptr_t pnew_page_addr = pmem_alloc();
    if (pnew_page_addr == 0) return false;
    vpte->phyaddr = pnew_page_addr >> 12;
    vpte->user_access = user ? 1 : 0;
    vpte->writable = writable ? 1 : 0;
    vpte->exe_disable = executable ? 0 : 1;
    vpte->present = 1;
  } else {
    kprintf("[WARNING] vm_map: Page is already mapped, vaddr = %p\n", vaddress);
  }
  return true;
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
    kprint_s("[ERROR] vm_unmap: proot is NULL\n");
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
  pml4_entry_t* vpml4e;
  pdpt_entry_t* vpdpte;
  pdpt_entry_t* vpdpt;
  pd_entry_t* vpde;
  pd_entry_t* vpd;
  pt_4kb_entry_t* vpte;
  pt_4kb_entry_t* vpt;

  // Access pml4
  vpml4e = (pml4_entry_t*)(proot + base_viraddr) + indices[4];

  // Access pdpt
  if (vpml4e->present == 1) {
    vpdpt = (pdpt_entry_t*)((vpml4e->pdpt_phyaddr << 12) + base_viraddr);
    vpdpte = vpdpt + indices[3];
  } else {
    return true;
  }

  // Access pd
  if (vpdpte->present == 1) {
    vpd = (pd_entry_t*)((vpdpte->pd_phyaddr << 12) + base_viraddr);
    vpde = vpd + indices[2];
  } else {
    return true;
  }

  // Access pt
  if (vpde->present == 1) {
    vpt = (pt_4kb_entry_t*)((vpd->pt_phyaddr << 12) + base_viraddr);
    vpte = vpt + indices[1];
  } else {
    return true;
  }

  // Unmap page and free allocated page
  if (vpte->present == 0) {
    return true;
  }
  pmem_free(vpte->phyaddr << 12);
  vpte->present = 0;
  // Check if all pt entries are not present, if so free the higher level table
  // entry
  for (int i = 0; i < NUM_PT_ENTRIES; i++) {
    if (vpt[i].present == 1) return true;
  }
  vmem_free((uintptr_t)vpt);
  vpde->present = 0;

  // Check if all pd entries are not present, if so free the higher level table
  // entry
  for (int i = 0; i < NUM_PT_ENTRIES; i++) {
    if (vpd[i].present == 1) return true;
  }
  vmem_free((uintptr_t)vpd);
  vpdpte->present = 0;

  // Check if all pdpt entries are not present, if so free the higher level
  // table entry
  for (int i = 0; i < NUM_PT_ENTRIES; i++) {
    if (vpdpt[i].present == 1) return true;
  }
  vmem_free((uintptr_t)vpdpt);
  vpml4e->present = 0;
  return true;
}

/**
 * Change the protection mode of the mapped page. If the virtual address is not
 * mapped, we return false. Return true if mode change success.
 */
bool vm_protect(uintptr_t proot, uintptr_t vaddress, bool user, bool writable,
                bool executable) {
  // Early exit if root address = 0
  if (proot == 0) {
    kprint_s("[ERROR] vm_protect: proot is NULL\n");
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
  pml4_entry_t* vpml4e;
  pdpt_entry_t* vpdpte;
  pd_entry_t* vpde;
  pt_4kb_entry_t* vpte;

  // Access pml4 entry
  vpml4e = (pml4_entry_t*)(proot + base_viraddr);
  vpml4e += indices[4];

  // Access pdpt entry
  if (vpml4e->present == 0) {
    kprint_s("[ERROR] vm_protect: PML4 entry not present\n");
    return false;
  } else {
    vpdpte = (pdpt_entry_t*)((vpml4e->pdpt_phyaddr << 12) + base_viraddr);
    vpdpte += indices[3];
  }

  // Access pd entry
  if (vpdpte->present == 0) {
    kprint_s("[ERROR] vm_protect: page dir pointer entry not present\n");
    return false;
  } else {
    vpde = (pd_entry_t*)((vpdpte->pd_phyaddr << 12) + base_viraddr);
    vpde += indices[2];
  }

  // Access pt entry
  if (vpde->present == 0) {
    kprint_s("[ERROR] vm_protect: page dir entry not present\n");
    return false;
  } else {
    vpte = (pt_4kb_entry_t*)((vpde->pt_phyaddr << 12) + base_viraddr);
    vpte += indices[1];
  }

  // Map the page to address space
  if (vpte->present == 0) {
    kprint_s("[ERROR] vm_protect: page table entry not present\n");
    return false;
  } else {
    vpte->accessed = user;
    vpte->writable = writable;
    vpte->exe_disable = !executable;
    return true;
  }
}

// Unmap everything in the lower half of an address space with level 4 page
// table at address root
void unmap_lower_half(uintptr_t root) {
  // We can reclaim memory used to hold page tables, but NOT the mapped pages
  pt_entry_t* l4_table = (pt_entry_t*)ptov(root);
  for (size_t l4_index = 0; l4_index < 256; l4_index++) {
    // Does this entry point to a level 3 table?
    if (l4_table[l4_index].present) {
      // Yes. Mark the entry as not present in the level 4 table
      l4_table[l4_index].present = false;

      // Now loop over the level 3 table
      pt_entry_t* l3_table =
          (pt_entry_t*)ptov(l4_table[l4_index].address << 12);
      for (size_t l3_index = 0; l3_index < 512; l3_index++) {
        // Does this entry point to a level 2 table?
        if (l3_table[l3_index].present && !l3_table[l3_index].page_size) {
          // Yes. Loop over the level 2 table
          pt_entry_t* l2_table =
              (pt_entry_t*)ptov(l3_table[l3_index].address << 12);
          for (size_t l2_index = 0; l2_index < 512; l2_index++) {
            // Does this entry point to a level 1 table?
            if (l2_table[l2_index].present && !l2_table[l2_index].page_size) {
              // Yes. Free the physical page the holds the level 1 table
              pmem_free(l2_table[l2_index].address << 12);
            }
          }
          // Free the physical page that held the level 2 table
          pmem_free(l3_table[l3_index].address << 12);
        }
      }
      // Free the physical page that held the level 3 table
      pmem_free(l4_table[l4_index].address << 12);
    }
  }
  // Reload CR3 to flush any cached address translations
  write_cr3(read_cr3());
}

/******************************************************************************/
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
  uint64_t proot = (uint64_t)read_cr3() & 0xFFFFFFFFFFFFF000;

  // Get address to the Page Map Level 4 entry
  // pml4[0:2] = 000
  // pml4[3:11] = address[39:47]
  // pml4[12:51] = cr3[12:51]
  pml4_entry_t* vpml4 = (pml4_entry_t*)(proot + base_viraddr) + indices[4];
  if (vpml4->present == 0) {
    kprintf("Memory not mapped at lv4: %p\n", vaddress);
    return;
  }

  // Get address to the Page Dir Pointer Table entry
  // pdpt[0:2] = 000
  // pdpt[3:11] = address[30:38]
  // pdpt[12:51] = pml4[12:51]
  pdpt_entry_t* vpdpt = (pdpt_entry_t*)((vpml4->pdpt_phyaddr << 12) +
                                        (indices[3] << 3) + base_viraddr);
  if (vpdpt->present == 0) {
    kprintf("Memory not mapped at lv3: %p\n", vaddress);
    return;
  }

  // Get address to the Page Dir entry
  // pd[0:2] = 000
  // pd[3:11] = address[21:29]
  // pd[12:51] = pdpt[12:51]
  pd_entry_t* vpd = (pd_entry_t*)((vpdpt->pd_phyaddr << 12) +
                                  (indices[2] << 3) + base_viraddr);
  if (vpd->present == 0) {
    kprintf("Memory not mapped at lv2: %p\n", vaddress);
    return;
  }

  // Get address to the Page Table
  // pt[0:2] = 000
  // pt[3:11] = address[12:20]
  // pt[12:51] = pd[12:51]
  pt_4kb_entry_t* vpt = (pt_4kb_entry_t*)((vpd->pt_phyaddr << 12) +
                                          (indices[1] << 3) + base_viraddr);
  if (vpt->present == 0) {
    kprintf("Memory not mapped at page table: %p\n", vaddress);
    return;
  }
  kprintf("address present %d\n", vpt->present);

  // Physical Address
  // phyaddr[0:11] = address[0:11]
  // phyaddr[12:51] = pt[12:51]
  uint64_t phy_page_num = vpt->phyaddr;
  uint64_t phyaddr = (phy_page_num << 12) + indices[0];

  // Print Level 4
  kprintf("  Level 4 (index %d of %p)\n", indices[4], proot);
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