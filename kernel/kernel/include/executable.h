#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "kprint.h"
#include "page.h"
#include "port.h"
#include "stivale2.h"
#include "util.h"

#define ET_EXEC 0x02

#define PT_NULL 0
#define PT_LOAD 1

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

// Functions

// Return the memory address of the executable's entry
bool load_executatble(const char* exe_name, exe_entry_fn_ptr_t* entry_func);
