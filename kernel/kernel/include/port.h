// Standard port input/output operations
// Source: https://wiki.osdev.org/Inline_Assembly/Examples#I.2FO_access
/**
 * By professor Charlie Curtsinger
 * src:
 * https://curtsinger.cs.grinnell.edu/teaching/2022S/CSC395/kernel/keyboard.html
 */
#pragma once

#include <stdint.h>

static inline void outb(uint16_t port, uint8_t val) {
  __asm__("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
  uint8_t ret;
  __asm__("inb %1, %0" : "=a"(ret) : "Nd"(port));
  return ret;
}

static inline uintptr_t read_cr3() {
  uintptr_t value;
  __asm__("mov %%cr3, %0" : "=r"(value));
  return value;
}

static inline void write_cr3(uint64_t value) {
  __asm__("mov %0, %%cr3" : : "r" (value));
}

static inline uint64_t read_cr0() {
  uintptr_t value;
  __asm__("mov %%cr0, %0" : "=r" (value));
  return value;
}

static inline void write_cr0(uint64_t value) {
  __asm__("mov %0, %%cr0" : : "r" (value));
}

static inline void io_wait() { outb(0x80, 0); }