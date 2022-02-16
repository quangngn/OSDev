#pragma once

#include <stddef.h>
#include <stdint.h>

// Halt the CPU in an infinite loop
static void halt() {
  while (1) {
    __asm__("hlt");
  }
}

/*
 * Return value from bit val[start:end]
 */
static inline uint64_t get_incl_bit(uint64_t val, uint64_t start,
                                    uint64_t end) {
  uint64_t mask = (((uint64_t)1 << (end - start + 1)) - 1) << start;
  return (val & mask) >> start;
}

/*
 * Return value from bit val[start:end-1]
 */
static inline uint64_t get_excl_bit(uint64_t val, uint64_t start,
                                    uint64_t end) {
  uint64_t mask = ((uint64_t)1 << (end - start)) << start;
  return (val & mask) >> start;
}

// Set memory to a certain value
static void* kmemset(void* ptr, int value, size_t num) {
  for (int i = 0; i < num; i++) {
    ((unsigned char*)ptr)[i] = (unsigned char)value;
  }
  return ptr;
}
