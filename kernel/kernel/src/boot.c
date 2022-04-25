#include <mem.h>
#include <process.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <system.h>

#include "executable.h"
#include "gdt.h"
#include "idt.h"
#include "kgraphic.h"
#include "kprint.h"
#include "page.h"
#include "pic.h"
#include "stivale2.h"
#include "syscall.h"
#include "term.h"
#include "util.h"

// Define struct tag pointer to hold information about memory section. These
// pointers will be init by reading provided struct tag from the bootloader.
struct stivale2_struct_tag_memmap* mmap_struct_tag = NULL;
struct stivale2_struct_tag_hhdm* hhdm_struct_tag = NULL;
struct stivale2_struct_tag_modules* modules_struct_tag = NULL;
struct stivale2_struct_tag_terminal* terminal_struct_tag = NULL;
struct stivale2_struct_tag_framebuffer* framebuffer_struct_tag = NULL;

// Function to write to terminal is defined in stivale2 source
extern term_write_t term_write;
extern int64_t syscall(uint64_t nr, ...);
extern uint8_t gdt;

// Reserve space for the stack
static uint8_t stack[8192];

/******************************************************************************/
// Tell booloader to unmap the lower part
static struct stivale2_tag unmap_null_hdr_tag = {
    .identifier = STIVALE2_HEADER_TAG_UNMAP_NULL_ID, .next = 0};

// Any video header tag with preference for a linear buffer
static struct stivale2_header_tag_any_video any_vid_hdr_tag = {
    .tag = {.identifier = STIVALE2_HEADER_TAG_ANY_VIDEO_ID,
            .next = (uintptr_t)(&unmap_null_hdr_tag)},
    .preference = 0};

static struct stivale2_header_tag_framebuffer framebuffer_hdr_tag = {
    .tag = {.identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
            .next = (uintptr_t)(&any_vid_hdr_tag)},
    .framebuffer_height = 0,
    .framebuffer_width = 0,
    .framebuffer_bpp = 0};

// Request a terminal from the bootloader
static struct stivale2_header_tag_terminal terminal_hdr_tag = {
    .tag = {.identifier = STIVALE2_HEADER_TAG_TERMINAL_ID,
            .next = (uintptr_t)(&framebuffer_hdr_tag)},
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

/******************************************************************************/
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

// void print_frame_buffer_info(struct stivale2_struct_tag_framebuffer* ftag) {
//   if (ftag == NULL) {
//     return;
//   }

//   kprintf("Frame buffer information: \n");
//   kprintf("Address: %p\n", ftag->framebuffer_addr);
//   kprintf("Dimension (W x H): %d x %d\n", ftag->framebuffer_width,
//           ftag->framebuffer_height);
//   kprintf("Bit per pixel: %d\n", ftag->framebuffer_bpp);
//   kprintf("Red mask size: %d, Red shift size: %d\n", ftag->red_mask_size,
//           ftag->red_mask_shift);
//   kprintf("Green mask size: %d, Green shift size: %d\n",
//   ftag->green_mask_size,
//           ftag->green_mask_shift);
//   kprintf("Blue mask size: %d, Blue shift size: %d\n", ftag->blue_mask_size,
//           ftag->blue_mask_shift);
// }

void struct_tag_setup(struct stivale2_struct* hdr) {
  // Look for a terminal tag
  terminal_struct_tag = find_tag(hdr, STIVALE2_STRUCT_TAG_TERMINAL_ID);
  // Make sure we find a terminal tag
  if (terminal_struct_tag == NULL) halt();
  // Save the term_write function pointer
  kset_term_write((term_write_t)terminal_struct_tag->term_write);

  // Mmap tag and HHDM tag:
  mmap_struct_tag = find_tag(hdr, STIVALE2_STRUCT_TAG_MEMMAP_ID);
  hhdm_struct_tag = find_tag(hdr, STIVALE2_STRUCT_TAG_HHDM_ID);

  // Module tag:
  modules_struct_tag = find_tag(hdr, STIVALE2_STRUCT_TAG_MODULES_ID);

  // Framebuffer tag:
  framebuffer_struct_tag = find_tag(hdr, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);
}

inline void enable_write_protection() {
  // Enable write protection
  uint64_t cr0 = read_cr0();
  cr0 |= 0x10000;
  write_cr0(cr0);
}

void invalidate_tlb(uintptr_t virtual_address) {
  __asm__("invlpg (%0)" ::"r"(virtual_address) : "memory");
}

void setup_kernel(struct stivale2_struct* hdr) {
  // We've booted! Let's start processing tags passed to kernel from the
  // bootloader
  struct_tag_setup(hdr);
  kprintf("Henlo!\n");

  // Init terminal
  graphic_init();
  term_init();
  kset_term_write((term_write_t)term_puts);

  // Set up the GDT
  gdt_setup();

  // Set up the IDT to handler interruption
  idt_setup();

  // Set up PIC
  pic_init();
  // Enable keyboard interrupt
  pic_unmask_irq(1);

  // Init free list for mapping paging
  init_free_list();

  // Enable write protection
  enable_write_protection();

  // Unmap lower half
  uintptr_t proot = read_cr3() & PAGE_ALIGN_MASK;
  unmap_lower_half(proot);

  // ((term_w_t)terminal_struct_tag->term_write)("Hello5\n", 8);

  // Init executable list for loading and running executable
  init_exe_list();
}

/******************************************************************************/
void _start(struct stivale2_struct* hdr) {
  // Set up kernel
  setup_kernel(hdr);

  // // Load and run shell program
  run_exe("shell");
  // print_frame_buffer_info(framebuffer_struct_tag);
  // ((term_w_t)terminal_struct_tag->term_write)("Hello\n", 7);
  // kprintf("Hello\n");

  // We're done, just hang...
  halt();
}
