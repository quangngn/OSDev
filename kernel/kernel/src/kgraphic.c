#include "kgraphic.h"

#include "psf.h"

extern struct stivale2_struct_tag_framebuffer* framebuffer_struct_tag;

size_t screen_w = 0;
size_t screen_h = 0;
uintptr_t buffer_addr = 0;

/**
 * Read the framebuffer struct tag to gain information about the current
 * framebuffer. The function also initialize psf font. 
 * \returns true if init successfully else returns false.
 */
bool kgraphic_init() {
  if (framebuffer_struct_tag == NULL) return false;

  // Init screen information
  screen_w = framebuffer_struct_tag->framebuffer_width;
  screen_h = framebuffer_struct_tag->framebuffer_height;
  buffer_addr = framebuffer_struct_tag->framebuffer_addr;

  // Init psf font
  if (!psf_init()) return false;

  return true;
}

/**
 * Set the framebuffer value to 0
 */ 
void kgraphic_clear_buffer() {
  uint64_t* cursor = (uint64_t*)buffer_addr;
  uint64_t* buffer_end_addr =
      (uint64_t*)(buffer_addr + (screen_w * screen_h * sizeof(pixel_t)));

  while (cursor < buffer_end_addr) {
    *cursor++ = 0;
  }
}