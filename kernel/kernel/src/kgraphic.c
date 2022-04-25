#include "kgraphic.h"
#include "psf.h"

extern struct stivale2_struct_tag_framebuffer* framebuffer_struct_tag;

size_t screen_w = 0;
size_t screen_h = 0;
uintptr_t buffer_addr = 0;

bool graphic_init() {
  if (framebuffer_struct_tag == NULL) return false;

  // Init screen information
  screen_w = framebuffer_struct_tag->framebuffer_width;
  screen_h = framebuffer_struct_tag->framebuffer_height;
  buffer_addr = framebuffer_struct_tag->framebuffer_addr;

  // Init psf font
  if (!psf_init()) return false;

  return true;
}

void graphic_clear_buffer() {
  uint64_t* cursor = (uint64_t*)buffer_addr;
  uint64_t* buffer_end_addr = (uint64_t*)(buffer_addr + (screen_w * screen_h));

  while (cursor < buffer_end_addr) {
    *cursor++ = 0;
  }
}