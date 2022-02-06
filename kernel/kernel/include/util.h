#pragma once

#include <stddef.h>
#include "util.h"

// Halt the CPU in an infinite loop
static void halt() {
  while (1) {
    __asm__("hlt");
  }
}

// Set memory to a certain value
static void* kmemset(void* ptr, int value, size_t num) {
  for (int i = 0; i < num; i++) {
    ((unsigned char*)ptr)[i] = (unsigned char) value;
  }
  return ptr;
}
