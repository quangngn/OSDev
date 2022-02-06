#include "kprint.h"

// Buffers being used to store digit characters when printing number
char buffer_dec_uint64[NUM_DIGIT_DEC_UINT64 + 1];
char buffer_hex_uint64[NUM_DIGIT_HEX_UINT64 + 1];

// Pointers to memmap struct tag and hhdm struct tag to help with printing
// memory usage
struct stivale2_struct_tag_memmap* mmap_struct_tag = NULL;
struct stivale2_struct_tag_hhdm* hhdm_struct_tag = NULL;


// Function to write to terminal. Init as NULL. Set after read terminal
// structure tag
term_write_t term_write = NULL;

// count the number of character in the input string
size_t kstrlen(const char* str) {
  const char* cursor = str;
  size_t len = 0;
  // Count the number of character before seeing '\0'
  while (*cursor != '\0') {
    len++;
    cursor++;
  }
  // Return the number of characters in str
  return len;
}

// Print a single character to the terminal
void kprint_c(char c) { term_write(&c, 1); }

// Print a string to the terminal
void kprint_s(const char* str) { term_write(str, kstrlen(str)); }

// Print an unsigned 64-bit integer value to the terminal in decimal notation
// (no leading zeros please!)
void kprint_d(uint64_t value) {
  if (value == 0) {
    kprint_c('0');
  } else {
    uint64_t remain = 0;
    size_t num_digit = 0;

    // using the global buffer to store the stringified decimal value
    buffer_dec_uint64[NUM_DIGIT_DEC_UINT64] = '\0';
    char* cursor = buffer_dec_uint64 + NUM_DIGIT_DEC_UINT64;

    // Fill in the buffer from the end to the beginning with each digit of value
    // (from right to left)
    while (value != 0) {
      remain = value % 10;
      value = value / 10;

      *(--cursor) = remain + '0';
      num_digit++;
    }

    term_write(cursor, num_digit);
  }
}

// Print an unsigned 64-bit integer value to the terminal in lowercase
// hexadecimal notation (no leading zeros or “0x” please!)
void kprint_x(uint64_t value) {
  if (value == 0) {
    kprint_c('0');
  } else {
    uint64_t remain = 0;
    size_t num_digit = 0;

    // using the global buffer to store the stringified hex value
    buffer_hex_uint64[NUM_DIGIT_HEX_UINT64] = '\0';
    char* cursor = buffer_hex_uint64 + NUM_DIGIT_HEX_UINT64;

    // Fill in the buffer from the end to the beginning with each digit of value
    // (from right to left)
    while (value != 0) {
      remain = value % 16;
      value = value / 16;

      // Decrease cursor by 1, set character based on the hex form of remain
      *(--cursor) = remain < 10 ? '0' + remain : 'a' - 10 + remain;
      num_digit++;
    }

    term_write(cursor, num_digit);
  }
}

// Print the value of a pointer to the terminal in lowercase hexadecimal with
// the prefix “0x”
void kprint_p(void* ptr) {
  kprint_s("0x");
  kprint_x((uint64_t)ptr);
}

// Print formatted string, supporting:
// - %s for string
// - %c for character
// - %d for decimal number
// - %x for hex number
// - %p for pointer address
void kprintf(const char* format, ...) {
  const char* cursor = format;
  // Set up va_list to read arguments
  va_list args;
  va_start(args, format);

  // Read each character in format and handle each case when we see %
  while (*cursor != '\0') {
    if (*cursor == '%') {
      // Process the character right after the '%'
      cursor++;
      switch (*cursor) {
        // case "%%" --> print '%'
        case '%':
          kprint_c('%');
          break;
        // case "%c" --> print next arg as a character
        case 'c':
          kprint_c(va_arg(args, int));
          break;
        // case "%s" --> print next arg as a string
        case 's':
          kprint_s(va_arg(args, char*));
          break;
        // case "%d" --> print next arg as a decimal number
        case 'd':
          kprint_d(va_arg(args, uint64_t));
          break;
        // case "%x" --> print next arg as a hex number
        case 'x':
          kprint_x(va_arg(args, uint64_t));
          break;
        // case "%p" --> print next arg as a pointer address
        case 'p':
          kprint_p(va_arg(args, void*));
          break;
        // case "%" -> print nothing, return
        case '\0':
          return;
        // unsupported escape character
        default:
          kprint_s("<not supported>");
      }
    } else {
      kprint_c(*cursor);
    }
    cursor++;
  }
}

// Print usable memory in format:
// "[P. Addr start]-[P. Addr end] mapped at [Vir. Addr start]-[Vir. Addr end]
void kprint_mem_usage() {
  // Check for null tag pointers
  if (mmap_struct_tag == NULL) {
    kprint_s("<Unable to identify memmap tag>\n");
    return;
  }
  if (hhdm_struct_tag == NULL) {
    kprint_s("<Unable to identify hhdm tag>\n");
    return;
  }
  
  // Get the higher half direct map virtual start address
  uint64_t hhdm_addr = hhdm_struct_tag->addr;

  // Find usable memory entries
  struct stivale2_mmap_entry* mmap_entry;
  // Loop through each entry in the memory
  for (size_t i = 0; i < mmap_struct_tag->entries; i++) {
    mmap_entry = &(mmap_struct_tag->memmap[i]);
    if (mmap_entry->type == STIVALE2_MMAP_TYPE_USABLE) {
      kprintf("%p-%p mapped at %p-%p\n", mmap_entry->base,
              mmap_entry->base + mmap_entry->length,
              mmap_entry->base + hhdm_addr,
              mmap_entry->base + mmap_entry->length + hhdm_addr);
    }
  }
}

// Set value to term_write
void kset_term_write(term_write_t fn) { term_write = fn; }

// Set value to mmap_struct_tag and hhdm_struct_tag
void kset_mem_struct_tags(struct stivale2_struct_tag_memmap *new_mmap_struct_tag,
                      struct stivale2_struct_tag_hhdm *new_hhdm_struct_tag) {
  mmap_struct_tag = new_mmap_struct_tag;
  hhdm_struct_tag = new_hhdm_struct_tag;
}