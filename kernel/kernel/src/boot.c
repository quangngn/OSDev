#include <stddef.h>
#include <stdint.h>

#include "executable.h"
#include "idt.h"
#include "kprint.h"
#include "page.h"
#include "pic.h"
#include "stivale2.h"
#include "syscall.h"
#include "util.h"

// Define struct tag pointer to hold information about memory section. These
// pointers will be init by reading provided struct tag from the bootloader.
struct stivale2_struct_tag_memmap* mmap_struct_tag = NULL;
struct stivale2_struct_tag_hhdm* hhdm_struct_tag = NULL;
struct stivale2_struct_tag_modules* modules_struct_tag = NULL;

// Function to write to terminal is defined in stivale2 source
extern term_write_t term_write;
extern int64_t syscall(uint64_t nr, ...);

// Reserve space for the stack
static uint8_t stack[8192];

// Tell booloader to unmap the lower part
static struct stivale2_tag unmap_null_hdr_tag = {
    .identifier = STIVALE2_HEADER_TAG_UNMAP_NULL_ID, .next = 0};

// Request a terminal from the bootloader
static struct stivale2_header_tag_terminal terminal_hdr_tag = {
    .tag = {.identifier = STIVALE2_HEADER_TAG_TERMINAL_ID,
            .next = (uintptr_t)(&unmap_null_hdr_tag)},
    .flags = 0};

// Declare the header for the bootloader
__attribute__((section(".stivale2hdr"),
               used)) static struct stivale2_header stivale_hdr = {
    // Use ELF file's default entry point
    .entry_point = 0,

    // Use stack (starting at the top)
    .stack = (uintptr_t)stack + sizeof(stack),

    // Bit 1: request pointers in the higher half
    // Bit 2: enable protected memory ranges (specified in PHDR)
    // Bit 3: virtual kernel mappings (no constraints on physical memory)
    // Bit 4: required
    .flags = 0x1E,

    // First tag struct
    .tags = (uintptr_t)&terminal_hdr_tag};

// Find a tag with a given ID
void* find_tag(struct stivale2_struct* hdr, uint64_t id) {
  // Start at the first tag
  struct stivale2_tag* current = (struct stivale2_tag*)hdr->tags;

  // Loop as long as there are more tags to examine
  while (current != NULL) {
    // Does the current tag match?
    if (current->identifier == id) {
      return current;
    }

    // Move to the next tag
    current = (struct stivale2_tag*)current->next;
  }

  // No matching tag found
  return NULL;
}

void term_setup(struct stivale2_struct* hdr) {
  // Look for a terminal tag
  struct stivale2_struct_tag_terminal* tag =
      find_tag(hdr, STIVALE2_STRUCT_TAG_TERMINAL_ID);

  // Make sure we find a terminal tag
  if (tag == NULL) halt();

  // Save the term_write function pointer
  kset_term_write((term_write_t)tag->term_write);
}

void mem_struct_setup(struct stivale2_struct* hdr) {
  // Look for memmap struct tag and hhdm struct tag to get the physical memmory
  mmap_struct_tag = find_tag(hdr, STIVALE2_STRUCT_TAG_MEMMAP_ID);
  hhdm_struct_tag = find_tag(hdr, STIVALE2_STRUCT_TAG_HHDM_ID);
}

void modules_struct_setup(struct stivale2_struct* hdr) {
  modules_struct_tag = find_tag(hdr, STIVALE2_STRUCT_TAG_MODULES_ID);
}

inline void enable_write_protection() {
  // Enable write protection
  uint64_t cr0 = read_cr0();
  cr0 |= 0x10000;
  write_cr0(cr0);
}

void setup_kernel(struct stivale2_struct* hdr) {
  // We've booted! Let's start processing tags passed to kernel from the
  // bootloader
  term_setup(hdr);
  mem_struct_setup(hdr);
  modules_struct_setup(hdr);

  // Set up the IDT to handler interruption
  idt_setup();

  // Set up PIC
  pic_init();
  // Enable keyboard interrupt
  pic_unmask_irq(1);

  // Enable write protection
  enable_write_protection();
}

void _start(struct stivale2_struct* hdr) {
  setup_kernel(hdr);

  exe_entry_fn_t entry_func = NULL;
  load_executatble("init", &entry_func);

  // Call entry function
  entry_func();

  // We're done, just hang...
  halt();
}
