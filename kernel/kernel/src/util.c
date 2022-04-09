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

/*
 * Return value from bit val[start:end]
 */
inline uint64_t get_incl_bit(uint64_t val, uint64_t start, uint64_t end) {
  uint64_t mask = (((uint64_t)1 << (end - start + 1)) - 1) << start;
  return (val & mask) >> start;
}

/*
 * Return value from bit val[start:end-1]
 */
inline uint64_t get_excl_bit(uint64_t val, uint64_t start, uint64_t end) {
  uint64_t mask = (((uint64_t)1 << (end - start)) - 1) << start;
  return (val & mask) >> start;
}

// Set memory to a certain value
void* kmemset(void* ptr, int value, size_t num) {
  for (int i = 0; i < num; i++) {
    ((unsigned char*)ptr)[i] = (unsigned char)value;
  }
  return ptr;
}

// Copy memory from src to dest. Size is in byte
void kmemcpy(void* dst, void* src, size_t size) {
  char* d = (char*)dst;
  char* s = (char*)src;

  for (int i = 0; i < size; i++) {
    *d++ = *s++;
  }
}

/**
 * Function to call string comparison. In case 1 string is the prefix of the
 * other string, the prefix is consider less than the other string.
 */
int kstrcmp(const char* str1, const char* str2) {
  int idx = 0;
  while (true) {
    if (str1[idx] < str2[idx]) {
      return -1;
    } else if (str1[idx] > str2[idx]) {
      return 1;
    } else if (str1[idx] == '\0' && str2[idx] == '\0') {
      // We reach the end of both strings and all characters are checked to be
      // the same
      return 0;
    }
    idx++;
  }
}

// Convert physical address to virtual address by adding hhdm base virtual
// address
uintptr_t ptov(uintptr_t paddr) { return paddr + hhdm_struct_tag->addr; }