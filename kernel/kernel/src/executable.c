#include "executable.h"

exe_info_t* exe_list = NULL;
exe_info_t* current_exe = NULL;

/******************************************************************************/
// Helper functions
/**
 * Create a exe_info_t struct using the given information.
 * \param exe_name Name of the executable.
 * \param entry Entry address of the executable.
 * \returns the new malloc struct, else return NULL.
 */  
exe_info_t* init_exe_info(const char* exe_name, exe_entry_fn_ptr_t entry) {
  printf("init_exe_info_t\n");
  exe_info_t* new_exe = (exe_info_t*)malloc(sizeof(exe_info_t));
  if (new_exe == NULL) return NULL;
  new_exe->exe_name = (char*)exe_name;
  new_exe->entry = entry;
  new_exe->segments = NULL;
  return new_exe;
}

/**
 * Free the malloc exe info struct, including its segment info structs.
 * \param pdel_exe pointer to the pointer of free exe info struct.
 */ 
void free_exe_info_struct(exe_info_t** pdel_exe) {
  printf("free_exe_info_t\n");
  if (pdel_exe == NULL || *pdel_exe == NULL) return;

  // Free each segment info struct
  exe_info_t* exe = *pdel_exe;
  seg_info_t* del_seg_info = NULL;
  while (exe->segments != NULL) {
    del_seg_info = exe->segments;
    exe->segments = exe->segments->next;
    free(del_seg_info);
  }
  // Free executable info struct
  free(exe);
  *pdel_exe = NULL;
}

exe_info_t* find_exe(const char* exe_name) {
  printf("find_exe_info_t\n");
  if (exe_name == NULL) return NULL;

  exe_info_t* exe = exe_list;
  while (exe != NULL) {
    if (strcmp(exe->exe_name, exe_name) == 0) {
      return exe;
    } 
    exe = exe->next;
  }
  return exe;
}

seg_info_t* init_seg_info(uintptr_t vaddr_seg, uintptr_t vaddr_seg_file,
                          size_t mem_size, size_t file_size, uint32_t prot) {
  printf("init_seg_info_t\n");
  seg_info_t* new_seg = (seg_info_t*)malloc(sizeof(seg_info_t));
  if (new_seg == NULL) return NULL;
  new_seg->vaddr_seg = vaddr_seg;
  new_seg->vaddr_seg_file = vaddr_seg_file;
  new_seg->mem_size = mem_size;
  new_seg->file_size = file_size;
  new_seg->readable = (prot & PROT_READ) != 0;
  new_seg->writable = (prot & PROT_WRITE) != 0;
  new_seg->executable = (prot & PROT_EXEC) != 0;
  new_seg->next = NULL;
  return new_seg;
}

/**
 * This function handles mapping memory for the segment and copy data from the
 * file image to the mapped memory region.
 *
 * \param segment_info Pointer to struct that hold important values to load
 * segment.
 * \returns true if load successfully, else returns false.
 */
bool load_segment(seg_info_t* segment_info) {
  printf("load_segment\n");
  // Extract variable from seg_info
  uintptr_t vaddr_seg = segment_info->vaddr_seg;
  uintptr_t vaddr_seg_file = segment_info->vaddr_seg_file;
  size_t mem_size = segment_info->mem_size;
  size_t file_size = segment_info->file_size;
  bool readable = segment_info->readable;
  bool writable = segment_info->writable;
  bool executable = segment_info->executable;

  // The segment might be bigger than a page size. As result, we need to map
  // multiple pages
  uintptr_t vaddr_cur_page = vaddr_seg & PAGE_ALIGN_MASK;
  uintptr_t vadd_end_seg = vaddr_seg + mem_size;
  // Get top table physical root address
  uintptr_t proot = read_cr3() & PAGE_ALIGN_MASK;

  // Divide segment to PAGE_SIZE chunk and map to a page in address space
  while (vaddr_cur_page < vadd_end_seg) {
    // Map the segment address to address space
    if (!vm_map(proot, vaddr_cur_page, true, true, false)) {
      perror("[ERROR] load_segment: Mapping segment failed at %p!\n",
             vaddr_cur_page);
      return false;
    }
    // Advance address to the next page
    vaddr_cur_page += PAGE_SIZE;
  }

  // Copy content from the file image to the newly mapped page
  kmemcpy((void*)vaddr_seg, (void*)(vaddr_seg_file), file_size);

  // After copying content to the newly mapped page, we set its protection
  // mode according to defined in ELF program header table entry
  vaddr_cur_page = vaddr_seg & PAGE_ALIGN_MASK;
  while (vaddr_cur_page < vadd_end_seg) {
    // Map the segment address to address space
    if (!vm_protect(proot, vaddr_cur_page, readable, writable, executable)) {
      perror("[ERROR] load_segment: Change protection failed at %p!\n",
             vaddr_cur_page);
      return false;
    }
    // Advance address to the next page
    vaddr_cur_page += PAGE_SIZE;
  }
  return true;
}

/**
 * This function handles unmapping memory for the segment.
 *
 * \param segment_info Pointer to struct that hold important values to unmap
 * segment.
 * \returns true if load successfully, else returns false.
 */
bool unmap_segment(seg_info_t* segment_info) {
  printf("cleanup_segment\n");
  // Extract variable from seg_info
  uintptr_t vaddr_seg = segment_info->vaddr_seg;
  uintptr_t vaddr_seg_file = segment_info->vaddr_seg_file;
  size_t mem_size = segment_info->mem_size;

  // The segment might be bigger than a page size. As result, we need to map
  // multiple pages
  uintptr_t vaddr_cur_page = vaddr_seg & PAGE_ALIGN_MASK;
  uintptr_t vadd_end_seg = vaddr_seg + mem_size;
  // Get top table physical root address
  uintptr_t proot = read_cr3() & PAGE_ALIGN_MASK;

  // Divide segment to PAGE_SIZE chunk and map to a page in address space
  while (vaddr_cur_page < vadd_end_seg) {
    // Map the segment address to address space
    if (!vm_unmap(proot, vaddr_cur_page)) {
      perror("[ERROR] unmap_segment: Unmapping segment failed!\n");
      return false;
    }
    // Advance address to the next page
    vaddr_cur_page += PAGE_SIZE;
  }
  return true;
}

/**
 * Run function in user mode
 */
void to_usermode(exe_entry_fn_ptr_t entry_func) {
  // Pick an arbitrary location and size for the user-mode stack
  uintptr_t user_stack = USER_STACK;
  size_t user_stack_size = 8 * PAGE_SIZE;

  // Map the user-mode-stack
  for (uintptr_t p = user_stack; p < user_stack + user_stack_size;
       p += 0x1000) {
    // Map a page that is user-accessible, writable, but not executable
    vm_map(read_cr3() & PAGE_ALIGN_MASK, p, true, true, false);
  }

  // And now jump to the entry point:
  // User data selector with priv=3
  // Stack starts at the high address minus 8 bytes
  // User code selector with priv=3
  // Jump to the entry point specified in the ELF file
  usermode_entry(USER_DATA_SELECTOR | 0x3, user_stack + user_stack_size - 8,
                 USER_CODE_SELECTOR | 0x3, entry_func);
}

/******************************************************************************/
// Main functions
/**
 * Loop find and extract necessary information of all executable image files
 * available to run. Store info struct in in the exe_list.
 * \returns true if execute successfully, else returns false.
 */
void init_exe_list() {
  printf("init_exe_list\n");
  // if (modules_struct_tag == NULL) {
  //   perror("[ERROR] load_exe_list: Unable to identify modules tag\n");
  //   return;
  // }
  // struct stivale2_module* module = modules_struct_tag->modules;

  // // Loop through each module to find the executable
  // for (int mod_idx = 0; mod_idx < modules_struct_tag->module_count; mod_idx++) {
  //   // Get executable's name:
  //   // 1. Read the ELF. e_hdr is also the vaddress of the start of the file
  //   elf_hdr_t* e_hdr = (elf_hdr_t*)module[mod_idx].begin;

  //   // 2. Check for executable type
  //   if (e_hdr->e_file_type != ET_EXEC) {
  //     continue;
  //   }

  //   // 3. Create new info struct for new executable
  //   exe_info_t* new_exe = init_exe_info(module[mod_idx].string, e_hdr->e_entry);
  //   if (new_exe == NULL) {
  //     perror("[ERROR] init_exe_list: Failed to load %s!\n",
  //            (exe_entry_fn_ptr_t)module[mod_idx].string);
  //     continue;
  //   }

  //   // 4. Get program header table
  //   prog_hdr_t* prog_hdr_tbl_base = NULL;
  //   // Make sure program header table offset is greater than 0
  //   if (e_hdr->e_prog_hdr_tbl_off == 0) {
  //     perror(
  //         "[ERROR] init_exe_list: Program header table offset is not greater "
  //         "than 0. Failed to load %s!\n",
  //         module[mod_idx].string);
  //     continue;
  //   } else {
  //     prog_hdr_tbl_base =
  //         (prog_hdr_t*)(e_hdr->e_prog_hdr_tbl_off + (uintptr_t)e_hdr);
  //   }

  //   // 5. Save information of each segment
  //   prog_hdr_t* cur_prog_hdr_entry = NULL;
  //   seg_info_t* new_seg = NULL;
  //   bool seg_load_error = false;
  //   int numb_segment = e_hdr->e_prog_hdr_tbl_ent_numb;
  //   for (int seg_idx = 0; seg_idx < numb_segment && !seg_load_error;
  //        seg_idx++) {
  //     // Offset to the table entry
  //     cur_prog_hdr_entry = &prog_hdr_tbl_base[seg_idx];

  //     // We only look for loadable segment for now
  //     if (cur_prog_hdr_entry->p_type != PT_LOAD) continue;

  //     // Loading and copying memory
  //     // Address of the segment in actual virtual memory address space.
  //     uintptr_t vaddr_seg = cur_prog_hdr_entry->p_vaddr;
  //     // Address of the segment in the image file. This is used for copying
  //     // the segment to vaddr_seg.
  //     uint64_t vaddr_seg_file = cur_prog_hdr_entry->p_offset + (uintptr_t)e_hdr;

  //     // Segment size in memory.
  //     size_t mem_size = cur_prog_hdr_entry->p_mem_size;
  //     // Segment size in image file. Might not include bss section.
  //     size_t file_size = cur_prog_hdr_entry->p_file_size;

  //     // Protection flag
  //     uint32_t prot = cur_prog_hdr_entry->p_flags;

  //     new_seg =
  //         init_seg_info(vaddr_seg, vaddr_seg_file, mem_size, file_size, prot);
  //     if (new_seg == NULL) {
  //       perror(
  //           "[ERROR] init_exe_list: Cannot load segment. Failed to load %s!\n",
  //           module[mod_idx].string);
  //       seg_load_error = true;
  //       break;  // break from looping through segment info
  //     }
  //     // Save new segment info new exe info struct
  //     new_seg->next = new_exe->segments;
  //     new_exe->segments = new_seg;
  //   }  // loop through segment info

  //   // 5.1 If we failed to load segmet info, and skip step 6
  //   if (seg_load_error) {
  //     free_exe_info_struct(&new_exe);
  //     continue;
  //   }

  //   // 6. Add the new exe info to the exe_list
  //   new_exe->next = exe_list;
  //   exe_list = new_exe;
  // }  // loop through module
}

/**
 * Function unmap executable from the memory. The input can either be exe_name
 * or the pointer to exe information struct (hint). The function would
 * prioritize using hint rather than exe_name.
 * \param exe_name Name of the executable.
 * \param hint Pointer to the executable info struct.
 * \returns true if unmap successfully, else returns false.
 */
bool load_exe(const char* exe_name, exe_entry_fn_ptr_t* entry_func) {
  printf("load_exe\n");
  if (exe_name == NULL || entry_func == NULL) {
    perror("[ERROR] load_exe: NULL input!\n");
    return false;
  }

  // 1. Find exe in exe_list using exe_name
  exe_info_t* cursor = find_exe(exe_name);
  if (cursor == NULL) {
    perror("[ERROR] load_exe: Cannot find executable with name %s!\n",
           exe_name);
    return false;
  }

  // 2. Load segment:
  seg_info_t* seg = cursor->segments;
  while (seg != NULL) {
    if (!load_segment(seg)) {
      perror("[ERROR] load_exe: Load Segment failed!\n");
      return false;
    }
    seg = seg->next;
  }

  // 3. Set entry function pointer to exe's entry,
  if (current_exe != NULL) {
    unmap_exe(NULL, current_exe);
  }
  current_exe = cursor;
  *entry_func = (exe_entry_fn_ptr_t)cursor->entry;
  return true;
}

bool unmap_exe(const char* exe_name, exe_info_t* hint) {
  printf("unmap_exe\n");
  if (exe_name == NULL && hint == NULL) {
    perror("[ERROR] unmap_exe: NULL inputs!\n");
    return false;
  }

  // 1. Find the exe to clean up
  exe_info_t* del_exe = exe_list;
  if (hint == NULL) {
    del_exe = find_exe(exe_name);
    if (del_exe == NULL) {
      perror("[ERROR] unmap_exe: Cannot find executable with name %s!\n",
             exe_name);
      return false;
    }
  } else {
    del_exe = hint;
  }

  // 2. Unmap each segment of the exe
  seg_info_t* seg = del_exe->segments;
  while (seg != NULL) {
    if (!unmap_segment(seg)) {
      perror("[ERROR] unmap_exe: Unable to unmap segment!\n");
      return false;
    }
    seg = seg->next;
  }
  return true;
}

/**
 * Find and run the executable with name exe_name.
 * \param exe_name Name of the executable.
 */
bool run_exe(const char* exe_name) {
  printf("run_exe\n");
  // Load executable
  exe_entry_fn_ptr_t fn;
  if (!load_exe(exe_name, &fn)) {
    return false;
  }
  to_usermode(fn);
  return true;
}

/**
 * Print information of the executable with name exe_name, including name and
 * each segment info.
 * \param exe_name Name of the executable.
 */
void print_exe_info(const char* exe_name) {
  exe_info_t* exe = find_exe(exe_name);
  if (exe == NULL) {
    perror("Cannot find executable with name: %s\n", exe_name);
    return;
  }

  // Print exe info:
  printf("Exe's name: %s.\n", exe_name);
  seg_info_t* seg = exe->segments;
  while (seg != NULL) {
    printf("|%p|%p|%d\t|%d\t|", seg->vaddr_seg, seg->vaddr_seg_file, seg->mem_size, seg->file_size);
    if(seg->readable) {
      printf("R");
    } else {
      printf("_");
    }

    if(seg->writable) {
      printf("W");
    } else {
      printf("_");
    }

    if(seg->executable) {
      printf("E");
    } else {
      printf("_");
    }
    printf("\n");
  }
}