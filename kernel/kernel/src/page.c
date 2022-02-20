#include "page.h"

// hhdm struct allow us to get the base virtual address
extern struct stivale2_struct_tag_hhdm* hhdm_struct_tag;
// memmap struct tag allows us to find the usable memory regions
extern struct stivale2_struct_tag_memmap* mmap_struct_tag;

// Pointer that point to the head of the page structure
page_4kb_t* freelist_header = NULL;

/**
 * Initialized the free list structure in USABLE memory sections. Each block of
 * this list would be 4kb.
 *
 * The fist
 */
bool init_free_list() {
  // If the struct tags are not found, we return false
  if (mmap_struct_tag == NULL || hhdm_struct_tag == NULL) {
    kprints("<Failed to read memory struct tags>\n");
    return false;
  } else {
    // Get base virtual address of hhdm
    uintptr_t based_vir_addr = hhdm_struct_tag->addr;

    // Find USABLE memory section
    struct stivale2_mmap_entry* mmap_entry;
    page_4kb_t* cursor = NULL;

    // Loop through each mem map section in mmap struct tag to find usable
    // section
    for (int i = 0; i < mmap_struct_tag->entries; i++) {
      mmap_entry = &(mmap_struct_tag->memmap[i]);
      if (mmap_entry->type == STIVALE2_MMAP_TYPE_USABLE) {
        // Init the value for block_header if the block_header is NULL
        if (freelist_header == NULL) {
          freelist_header = (page_4kb_t*)(mmap_entry->base + based_vir_addr);
        }

        // Init the cursor value. If the cursor is NULL (meaning this is the
        // first section), cursor = base address of the section. If cursor is
        // not NULL, (meaning it is pointing to the end of the previous cursor),
        // we set the next pointer (aka the first element of block array) to the
        // address of current section base and advance the cursor.
        if (cursor == NULL) {
          cursor = freelist_header;
        } else {
          cursor->elems[0] = mmap_entry->base + based_vir_addr;
          cursor = (page_4kb_t*)(mmap_entry->base + based_vir_addr);
        }

        // Set up free list in the current usable memory section
        while ((uintptr_t)(cursor + 1) <
               mmap_entry->base + mmap_entry->length) {
          // elems[0] of the freelist stores the address of the next block
          cursor->elems[0] = (uintptr_t)(cursor + 1);
          // elems[1] of the freelist stores the magic number
          cursor->elems[1] = MAGIC_NUM;
          // Advance cursor to the next block
          cursor++;
        }
      }
    }
  }
}

/**
 * Allocate a page of physical memory.
 * \returns the physical address of the allocated physical memory or 0 on error.
 */
uintptr_t pmem_alloc() {
  if (freelist_header == NULL) {
    return NULL;
  } else {
    uintptr_t ret = (uintptr_t)(&freelist_header);
    freelist_header = (page_4kb_t*)(freelist_header->elems[0]);
    // Overwrite the MAGIC NUM
    ((page_4kb_t*)ret)->elems[1] = 0;
    return ret;
  }
}

/**
 * Free a page of physical memory.
 * \param p is the physical address of the page to free, which must be
 * page-aligned.
 */
void pmem_free(uintptr_t p) {
  // Early return of p is NULL
  if (p == NULL || p % PAGE_SIZE != 0) {
    kprint_s("The pointer p is either NULL or not page aligned!\n");
    return;
  }

  // Treat p as a pointer to the block_4kb_t. Put this block at the top of the
  // free list.
  page_4kb_t* temp = (page_4kb_t*)p;
  // Avoiding double free by checking the magic num;
  if (temp->elems[1] == MAGIC_NUM) {
    return;
  }
  // Update the free list' new header
  temp->elems[0] = (uintptr_t)freelist_header;
  temp->elems[1] = MAGIC_NUM;
  freelist_header = temp;
}

/**
 * Map a single page of memory into a virtual address space.
 * \param root The physical address of the top-level page table structure
 * \param address The virtual address to map into the address space, must be
 * page-aligned
 * \param user Should the page be user-accessible?
 * \param writable
 * Should the page be writable?
 * \param executable Should the page be executable?
 * \returns true if the mapping succeeded, or false if there was an error
 */
bool vm_map(uintptr_t root, uintptr_t address, bool user, bool writable,
            bool executable) {
  // Early exit if root address = 0
  if (root == 0) {
    kprint_s("[vm_map FAILED] root is NULL\n");
    return false;
  }

  // Early exit if address is not page aligned
  if (address & 0x7 != 0) {
    kprints("[vm_map FAILED] address is not page aligned\n");
    return false;
  }

  // Get the based address given by hhdm. In this case we assume that it is in
  // section 0
  uint64_t base_viraddr = hhdm_struct_tag->addr;

  // Make an array of paging structure's entries indices from input address
  uint16_t indices[] = {
      (uint64_t)address & 0xFFF, ((uint64_t)address >> 12) & 0x1FF,
      ((uint64_t)address >> 21) & 0x1FF, ((uint64_t)address >> 30) & 0x1FF,
      ((uint64_t)address >> 39) & 0x1FF};

  // Declare paging structure pointers
  pml4_entry_t* pml4e;
  pdpt_entry_t* pdpte;
  pd_entry_t* pde;
  pt_4kb_entry_t* pte;

  // Access pml4
  pml4e = (pml4_entry_t*)((root << 12) + indices[4] + base_viraddr);

  // Access pdpte
  if (pml4e->present == 0) {
    // Allocate place for new pdpt and set its entries to 0
    pdpte = (pdpt_entry_t*)pmem_alloc();
    if (pdpte == NULL) return false;
    pml4e->pdpt_phyaddr = (uint64_t)pdpte >> 12;
    for (int i = 0; i < NUM_PT_ENTRIES; i++) ((uint64_t*)pdpte)[i] = 0;
    pdpte += indices[3];
    // Update pml4e
    pml4e->present = 1;
    pml4e->user_access = 1;
    pml4e->writable = 1;
    pml4e->exe_disable = 0;
  } else {
    pdpte = (pdpt_entry_t*)((pml4e->pdpt_phyaddr << 12) + indices[3] +
                            base_viraddr);
  }

  // Access pd
  if (pdpte->present == 0) {
    // Allocate place for new pd and set its entries to 0
    pde = (pd_entry_t*)pmem_alloc();
    if (pde == NULL) return false;
    pdpte->pd_phyaddr = (uint64_t)pde >> 12;
    for (int i = 0; i < NUM_PT_ENTRIES; i++) ((uint64_t*)pde)[i] = 0;
    pde += indices[2];
    // Update pdpte
    pdpte->present = 1;
    pdpte->user_access = 1;
    pdpte->writable = 1;
    pdpte->exe_disable = 0;
  } else {
    pde = (pd_entry_t*)((pdpte->pd_phyaddr << 12) + indices[2] + base_viraddr);
  }

  // Access pt
  if (pde->present == 0) {
    // Allocate place for new pt and set its entries to 0
    pte = (pt_4kb_entry_t*)pmem_alloc();
    if (pte == NULL) return false;
    pde->pt_phyaddr = (uint64_t)pte >> 12;
    for (int i = 0; i < NUM_PT_ENTRIES; i++) ((uint64_t*)pte)[i] = 0;
    pte += indices[1];
    // Update pde
    pde->present = 1;
    pde->user_access = 1;
    pde->writable = 1;
    pde->exe_disable = 0;
  } else {
    pte =
        (pt_4kb_entry_t*)((pde->pt_phyaddr << 12) + indices[1] + base_viraddr);
  }

  // Map the page to address space
  if (pte->present == 0) {
    pte->user_access = user ? 1 : 0;
    pte->writable = writable ? 1 : 0;
    pte->exe_disable = executable ? 0 : 1;
    pte->phyaddr = indices[0];
    return true;
  } else {
    kprints("[vm_map FAILED]Page is already mapped\n");
    return false;
  }
}

/**
 * Unmap the page from the memory address space
 */
bool vm_map(uintptr_t root, uintptr_t address) {
  // Early exit if root address = 0
  if (root == 0) {
    kprint_s("[vm_map FAILED] root is NULL\n");
    return false;
  }

  // Early exit if address is not page aligned
  if (address & 0x7 != 0) {
    kprints("[vm_map FAILED] address is not page aligned\n");
    return false;
  }

  // Get the based address given by hhdm. In this case we assume that it is in
  // section 0
  uint64_t base_viraddr = hhdm_struct_tag->addr;

  // Make an array of paging structure's entries indices from input address
  uint16_t indices[] = {
      (uint64_t)address & 0xFFF, ((uint64_t)address >> 12) & 0x1FF,
      ((uint64_t)address >> 21) & 0x1FF, ((uint64_t)address >> 30) & 0x1FF,
      ((uint64_t)address >> 39) & 0x1FF};

  // Declare paging structure pointers
  pml4_entry_t* pml4e;
  pdpt_entry_t* pdpte;
  pdpt_entry_t* pdpt;
  pd_entry_t* pde;
  pd_entry_t* pd;
  pt_4kb_entry_t* pte;
  pt_4kb_entry_t* pt;

  // Access pml4
  pml4e = (pml4_entry_t*)((root << 12) + indices[4] + base_viraddr);

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
    pte = pt + indices[0];
  } else {
    return true;
  }

  // Unmap page
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
      (uint64_t)address & 0xFFF, ((uint64_t)address >> 12) & 0x1FF,
      ((uint64_t)address >> 21) & 0x1FF, ((uint64_t)address >> 30) & 0x1FF,
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