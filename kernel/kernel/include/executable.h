#pragma once
#include <mem.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "gdt.h"
#include "kprint.h"
#include "page.h"
#include "port.h"
#include "stivale2.h"
#include "util.h"
#include "usermode_entry.h"

// ELF file type
#define ET_EXEC 0x02

// Program table type
#define PT_NULL 0
#define PT_LOAD 1

// Program table flag (for permission)
#define PF_X 0x1
#define PF_W 0x2
#define PF_R 0x4

typedef void (*exe_entry_fn_ptr_t)();

// Extern modules struct tag defined in boot.c
extern struct stivale2_struct_tag_modules* modules_struct_tag;

/**
 * Structs to define the structure of an ELF file.
 * Reference: https://en.wikipedia.org/wiki/Executable_and_Linkable_Format
 */
typedef struct elf_hdr {
  uint8_t e_id[16];      // ELF identifications.
  uint16_t e_file_type;  // Object file type (ex: executable file, relocatable
                         // file ...).
  uint16_t e_machine;    // Type of supported ISA (ex: x86, ARM ...).
  uint32_t e_version;    // Set to 1 for the original version of ELF.

  uint64_t e_entry;  // Entry point where the process starts executing.
  uint64_t e_prog_hdr_tbl_off;  // Offset to the Program Header Table.
  uint64_t e_sec_hdr_tbl_off;   // Offset to the Section Header Table.

  uint32_t e_flags;     // Interpretation of this field depends on the target
                        // architecture.
  uint16_t e_hdr_size;  // Size of ELF header (64 bytes for 64-bit and 52 bytes
                        // for 32-bit).
  uint16_t e_prog_hdr_tbl_ent_size;  // Size of the program header table entry
  uint16_t e_prog_hdr_tbl_ent_numb;  // Number of entries in the program
                                     // header table.
  uint16_t e_sec_hdr_tbl_ent_size;   // Size of the section header table entry
  uint16_t e_sec_hdr_tbl_ent_numb;   // Number of entries in the section header
                                     // table.

  uint16_t e_sec_name_str_tbl_idx;  // Contains index of the section header
                                    // table entry that contains the
                                    // section names.
} elf_hdr_t;

// Execution would care about segment
typedef struct prog_hdr {
  uint32_t p_type;       // Type of the segment.
  uint32_t p_flags;      // Segment-dependent flags.
  uint64_t p_offset;     // Offset of the segment in the file image.
  uint64_t p_vaddr;      // Virtual address of the segment in memory.
  uint64_t p_paddr;      // Segment's physical address.
  uint64_t p_file_size;  // Size in bytes of the segment in the file image.
                         // May be 0.
  uint64_t p_mem_size;   // Size in bytes of the segment in memory. May be 0.
  uint64_t p_align;      // 0 and 1 specify no alignment. Otherwise should be a
                         // positive, integral power of 2, with p_vaddr equating
                         // p_offset modulus p_align.
} prog_hdr_t;

// Linker would care about the section
typedef struct sec_hdr {
  uint32_t sh_name;    // An offset to a string in the .shstrtab section that
                       // represents the name of this section.
  uint32_t sh_type;    // Type of the header (ex: Prog data, Symbol table, ...).
  uint64_t sh_flags;   // Attributes of the section (ex: Writable, ...).
  uint64_t sh_vaddr;   // Virtual address of the section in memory, for sections
                       // that are loaded.
  uint64_t sh_offset;  // Offset of the section in the file image.

  uint64_t sh_size;  // Size (byte) of the section in the file image. May be 0.
  uint32_t sh_link;  // Section index of an associated section. This field is
                     // used for several purposes, depending on the type of
                     // section.
  uint32_t sh_info;  // Extra information about the section. This field is used
                     // for several purposes, depending on the type of section.
  uint64_t sh_addralign;  // Required alignment of the section. This field must
                          // be a power of two.
  uint64_t sh_ent_size;  // Contains the size (byte) of each entry, for sections
                         // that contain fixed-size entries. Otherwise, this
                         // field contains zero.
} sec_hdr_t;

// Struct contains information necessary to load segment to mem space
typedef struct seg_info {
  uintptr_t vaddr_seg;
  uintptr_t vaddr_seg_file;
  size_t mem_size;
  size_t file_size;
  bool readable;
  bool writable;
  bool executable;
  struct seg_info* next;
} seg_info_t;

// Struct contains information to run and executable
typedef struct exe_info {
  char* exe_name;
  exe_entry_fn_ptr_t entry;
  seg_info_t* segments;
  struct exe_info* next;
} exe_info_t;

/******************************************************************************/
/**
 * Loop find and extract necessary information of all executable image files
 * available to run. Store info struct in in the exe_list.
 * \returns true if execute successfully, else returns false.
 */
void init_exe_list();

/**
 * Function loads the first executable with matching name exe_name from stivale2
 * module. The entry_func is going to be set to the entry address of the
 * executable.
 * \param exe_name Name of the executable.
 * \param exe_entry_fn_ptr_t Function pointer to be set.
 * \returns true if load successfully, else returns false.
 */
bool load_exe(const char* exe_name, exe_entry_fn_ptr_t* entry_func);

/**
 * Function unmap executable from the memory. The input can either be exe_name
 * or the pointer to exe information struct (hint). The function would
 * prioritize using hint rather than exe_name.
 * \param exe_name Name of the executable.
 * \param hint Pointer to the executable info struct.
 * \returns true if unmap successfully, else returns false.
 */
bool unmap_exe(const char* exe_name, exe_info_t* hint);

/**
 * Find and run the executable with name exe_name.
 * \param exe_name Name of the executable.
 */
bool run_exe(const char* exe_name);

/**
 * Print information of the executable with name exe_name, including name and
 * each segment info.
 * \param exe_name Name of the executable.
 */
void print_exe_info(const char* exe_name);
