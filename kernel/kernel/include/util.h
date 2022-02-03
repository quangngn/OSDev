#pragma once

#include <stddef.h>

// Type of function to print to stivale2's terminal
typedef void (*term_write_t)(const char*, size_t);

// Halt the CPU in an infinite loop
static void halt() {
  while (1) {
    __asm__("hlt");
  }
}
