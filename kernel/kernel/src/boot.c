#include <stddef.h>
#include <stdint.h>

#include "kprint.h"
#include "stivale2.h"
#include "util.h"

// Function to write to stivale2 terminal
term_write_t term_write = NULL;

// Reserve space for the stack
static uint8_t stack[8192];

// Request a terminal from the bootloader
static struct stivale2_header_tag_terminal terminal_hdr_tag = {
    .tag = {.identifier = STIVALE2_HEADER_TAG_TERMINAL_ID, .next = 0},
    .flags = 0};

// Declare the header for the bootloader
__attribute__((section(".stivale2hdr"), used)) 
static struct stivale2_header stivale_hdr = {
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
  term_write = (term_write_t)tag->term_write;
}

void _start(struct stivale2_struct* hdr) {
  // We've booted! Let's start processing tags passed to use from the bootloader
  term_setup(hdr);

  // Print a greeting
  term_write("Hello Kernel!\n", 14);
  

  // We're done, just hang...
  halt();
}
