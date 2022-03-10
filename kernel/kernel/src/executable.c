#include "executable.h"

void print_module_info(struct stivale2_module* module) {
  kprintf("%s: start from %p to %p.\n", module->string, module->begin,
          module->end);
}

bool load_segment(uintptr_t vaddr_seg, uint64_t size,
                  uintptr_t vaddr_seg_file_location, bool readable,
                  bool writable, bool executable) {
  // The segment might be bigger than a page size. As result, we need to map
  // multiple page
  uintptr_t vaddr_cur_page = vaddr_seg;
  uintptr_t vadd_end_seg = vaddr_seg + size;

  // Get top table physical root address
  uintptr_t proot = read_cr3() & 0xFFFFFFFFFFFFF000;

  // Divide segment to PAGE_SIZE chunk and map to a page in address space
  while (vaddr_cur_page < vadd_end_seg) {
    // Map the segment address to address space
    if (!vm_map(proot, vaddr_cur_page, true, true, false)) {
      kprintf("[ERROR] load_segment: Mapping segment failed, vaddr = %p\n",
              vaddr_cur_page);
      return false;
    }
    kprintf("Map segment address %p\n", vaddr_cur_page);

    // Copy content from the file image to the newly mapped page
    kmemcpy((void*)vaddr_cur_page, (void*)vaddr_seg_file_location, PAGE_SIZE);

    // After copying content to the newly mapped page, we set its protection
    // mode according to defined in ELF program header table entry
    if (!vm_protect(proot, vaddr_cur_page, readable, writable, executable)) {
      kprintf("[ERROR] load_segment: Change protection failed, vaddr = %p\n",
              vaddr_cur_page);
      return false;
    }

    // Advance address to the next page
    vaddr_cur_page += PAGE_SIZE;
  }
  return true;
}

bool load_executatble(const char* exe_name, exe_entry_fn_t* entry_func) {
  if (modules_struct_tag == NULL) {
    kprint_s("[Error] load_executatble: Unable to identify modules tag\n");
    return false;
  }

  struct stivale2_module* module = modules_struct_tag->modules;

  // Loop through each module to find the executable
  for (int mod_idx = 0; mod_idx < modules_struct_tag->module_count; mod_idx++) {
    if (exe_name != NULL && strcmp(module[mod_idx].string, exe_name) == 0) {
      // Loading executable:

      // 1. Read the ELF. e_hdr is also the vaddress of the start of the file
      elf_hdr_t* e_hdr = (elf_hdr_t*)module[mod_idx].begin;

      // 2. Check for executable.
      if (e_hdr->e_file_type != ET_EXEC) {
        continue;
      }

      // 3. Find the program header table.
      int numb_segment = e_hdr->e_prog_hdr_tbl_ent_numb;
      prog_hdr_t* prog_hdr_tbl_base = NULL;
      // Make sure program header table offset is greater than 0
      if (e_hdr->e_prog_hdr_tbl_off == 0) {
        continue;
      } else {
        prog_hdr_tbl_base =
            (prog_hdr_t*)(e_hdr->e_prog_hdr_tbl_off + (uintptr_t)e_hdr);
      }

      // 4. Copy each segment to new memory location, map using vm_map.
      prog_hdr_t* cur_prog_hdr_entry = NULL;
      for (int seg_idx = 0; seg_idx < numb_segment; seg_idx++) {
        // Offset to the table entry
        cur_prog_hdr_entry = &prog_hdr_tbl_base[seg_idx];

        // We only look for loadable segment for now
        if (cur_prog_hdr_entry->p_type != PT_LOAD) continue;

        // Loading and copying memory
        uintptr_t vaddr_seg = cur_prog_hdr_entry->p_vaddr;
        uint64_t size = cur_prog_hdr_entry->p_mem_size;
        uint32_t flags = cur_prog_hdr_entry->p_flags;

        bool executable = (flags & PF_X) != 0;
        bool writable = (flags & PF_W) != 0;
        bool readable = (flags & PF_R) != 0;
        // Address of the segment in the image file. This is used for copying
        // the segment to new address
        uint64_t vaddr_seg_file_location =
            cur_prog_hdr_entry->p_offset + (uintptr_t)e_hdr;

        // Map and copy segment to a new address
        if (!load_segment(vaddr_seg, size, vaddr_seg_file_location, readable,
                          writable, executable)) {
          kprintf("[ERROR] load_executatble: Load Segment failed, vaddr = % p\n",
                  vaddr_seg);
          return false;
        }
        kprintf("Seg type %d, size %d, vaddress %p, flags %d\n",
                cur_prog_hdr_entry->p_type, size, vaddr_seg, flags);
      }

      // 5. Set entry function address and return
      // Set entry function address
      *entry_func = (exe_entry_fn_t)e_hdr->e_entry;
      return true;
    }
  }
  kprintf("[ERROR] load_executatble: Cannot find executable: %s\n", exe_name);
  return false;
}