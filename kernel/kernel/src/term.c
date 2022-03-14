#include "term.h"

// hhdm struct allow us to get the base virtual address
extern struct stivale2_struct_tag_hhdm* hhdm_struct_tag;

// A pointer to the VGA buffer
vga_entry_t* term;

// The current cursor position in the terminal
size_t term_col = 0;
size_t term_row = 0;

// Turn on the VGA cursor
void term_enable_cursor() {
  // Set starting scaline to 13 (three up from bottom)
  outb(0x3D4, 0x0A);
  outb(0x3D5, (inb(0x3D5) & 0xC0) | 13);

  // Set ending scanline to 15 (bottom)
  outb(0x3D4, 0x0B);
  outb(0x3D5, (inb(0x3D5) & 0xE0) | 15);
}

// Update the VGA cursor
void term_update_cursor() {
  uint16_t pos = term_row * VGA_WIDTH + term_col;

  outb(0x3D4, 0x0F);
  outb(0x3D5, (uint8_t)(pos & 0xFF));
  outb(0x3D4, 0x0E);
  outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

// Clear the terminal
void term_clear() {
  // Clear the terminal
  for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
    term[i].c = ' ';
    term[i].bg = VGA_COLOR_BLACK;
    term[i].fg = VGA_COLOR_WHITE;
  }

  term_col = 0;
  term_row = 0;

  term_update_cursor();
}

// Write one character to the terminal
void term_putchar(char c, uint8_t fg, uint8_t bg) {
  // Handle characters that do not consume extra space (no scrolling necessary)
  if (c == '\r') {
    term_col = 0;
    term_update_cursor();
    return;

  } else if (c == '\b') {
    if (term_col > 0) {
      term_col--;
      term[term_row * VGA_WIDTH + term_col].c = ' ';
    }
    term_update_cursor();
    return;
  }

  // Handle newline
  if (c == '\n') {
    term_col = 0;
    term_row++;
  }

  // Wrap if needed
  if (term_col == VGA_WIDTH) {
    term_col = 0;
    term_row++;
  }

  // Scroll if needed
  if (term_row == VGA_HEIGHT) {
    // Shift characters up a row
    kmemcpy(term, &term[VGA_WIDTH],
            sizeof(vga_entry_t) * VGA_WIDTH * (VGA_HEIGHT - 1));
    term_row--;

    // Clear the last row
    for (size_t i = 0; i < VGA_WIDTH; i++) {
      size_t index = i + term_row * VGA_WIDTH;
      term[index].c = ' ';
      term[index].fg = fg;
      term[index].bg = bg;
    }
  }

  // Write the character, unless it's a newline
  if (c != '\n') {
    size_t index = term_col + term_row * VGA_WIDTH;
    term[index].c = c;
    term[index].fg = fg;
    term[index].bg = bg;
    term_col++;
  }

  term_update_cursor();
}

// Write string to the terminal
void term_puts(const char* s, size_t size, uint8_t fg, uint8_t bg) {
  for (int i = 0; i < size; i++) {
    term_putchar(s[i], fg, bg);
  }
}

// Initialize the terminal
void term_init() {
  // Get a usable pointer to the VGA text mode buffer
  term = (vga_entry_t*)(VGA_BUFFER + hhdm_struct_tag->addr);

  term_enable_cursor();
  term_clear();
}
