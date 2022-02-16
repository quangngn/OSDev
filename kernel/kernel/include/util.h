#pragma once

#include <stddef.h>
#include <stdint.h>

// Halt the CPU in an infinite loop
static void halt() {
  while (1) {
    __asm__("hlt");
  }
}

static inline uint64_t get_incl_bit(uint64_t val, uint64_t bit_left,
                                    uint64_t bit_right) {
  uint64_t mask = (((uint64_t)1 << (bit_right - bit_left + 1)) - 1) << bit_left;
  return (val & mask) >> bit_left;
}

static inline uint64_t get_excl_bit(uint64_t val, unsigned char bit_left,
                                    unsigned char bit_right) {
  uint64_t mask = ((uint64_t)1 << (bit_right - bit_left)) << bit_left;
  return (val & mask) >> bit_left;
}

// Set memory to a certain value
static void* kmemset(void* ptr, int value, size_t num) {
  for (int i = 0; i < num; i++) {
    ((unsigned char*)ptr)[i] = (unsigned char)value;
  }
  return ptr;
}
