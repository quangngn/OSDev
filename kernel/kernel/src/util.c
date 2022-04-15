#include "util.h"

#include "kprint.h"

// hhdm struct allow us to get the base virtual address
extern struct stivale2_struct_tag_hhdm* hhdm_struct_tag;

// Halt the CPU in an infinite loop
void halt() {
  while (1) {
    __asm__("hlt");
  }
}

// Convert physical address to virtual address by adding hhdm base virtual
// address
uintptr_t ptov(uintptr_t paddr) { return paddr + hhdm_struct_tag->addr; }