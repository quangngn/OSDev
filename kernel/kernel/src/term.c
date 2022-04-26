/**
 * From professor Charlie Curtsinger
 * src:
 * https://curtsinger.cs.grinnell.edu/teaching/2022S/CSC395/kernel/housekeeping.html
 */
#include "term.h"

// hhdm struct allow us to get the base virtual address
extern struct stivale2_struct_tag_hhdm* hhdm_struct_tag;
extern struct stivale2_struct_tag_framebuffer* framebuffer_struct_tag;

// Defined in psf.c
extern uint32_t psf_font_w;
extern uint32_t psf_font_h;
extern size_t term_h;
extern size_t term_w;

// Defined in kgraphic.c
extern size_t screen_h;
extern size_t screen_w;
extern uintptr_t buffer_addr;

// Struct to hold the current state of the terminal
terminal_t term;

/******************************************************************************/
// Initialize the terminal
bool term_init() {
  if (framebuffer_struct_tag == NULL) return false;

  // Init terminal's state values
  term.row = 0;
  term.col = 0;
  term.fg = ARGB32_WHITE;
  term.bg = ARGB32_BLACK;
  term.enable_cursor = true;
  // Number of byte in frame buffer buffer for each row of the terminal
  term.byte_per_row = screen_w * psf_font_h * sizeof(pixel_t);

  // Clear terminal (aka clear frame buffer)
  term_clear();
  return true;
}

void term_reset_color() {
  term.fg = ARGB32_WHITE;
  term.bg = ARGB32_BLACK;
}

void term_set_color(color_t new_fg, color_t new_bg) {
  term.fg = new_fg;
  term.bg = new_bg;
}

// Clear the terminal
void term_clear() {
  // Clear the framebuffer
  graphic_clear_buffer();
  term.col = 0;
  term.row = 0;
  term.fg = ARGB32_WHITE;
  term.bg = ARGB32_BLACK;
  term.enable_cursor = true;
  // Reset the terminal stats
  term.byte_per_row = screen_w * psf_font_h * sizeof(pixel_t);
}

// Draw psf glyph of character c to the terminal
bool term_put_psf_char(char c) {
  return psf_put_char(c, term.row * psf_font_h, term.col * psf_font_w, term.fg,
                      term.bg);
}

// Write one character to the terminal
void term_putchar(char c) {
  // Handle characters that do not consume extra space (no scrolling necessary)
  if (c == '\r') {
    term.col = 0;
    return;
  } else if (c == '\b') {
    if (term.col > 0) {
      term.col--;
      term_put_psf_char(' ');
    } else if (term.row > 0) {
      term.row--;
      term.col = term_w - 1;
      term_put_psf_char(' ');
    }
    return;
  }

  // Handle newline
  if (c == '\n') {
    term.col = 0;
    term.row++;
  }

  // Wrap if needed
  if (term.col == term_w) {
    term.col = 0;
    term.row++;
  }

  // Scroll if needed
  if (term.row == term_h) {
    // Shift characters up a row
    uintptr_t src = buffer_addr + term.byte_per_row;
    size_t copy_size = term.byte_per_row * (term_h - 1);
    kmemcpy((void*)buffer_addr, (void*)src, copy_size);

    // Clear the last row
    src = buffer_addr + copy_size;
    kmemset((void*)src, 0, term.byte_per_row);

    // Set cursor to the start of the bottom line
    term.row--;
    term.col = 0;
  }

  // Write the character, unless it's a newline
  if (c != '\n') {
    term_put_psf_char(c);
    term.col++;
  }
}

// Write string to the terminal
void term_puts(const char* s, size_t size) {
  for (int i = 0; i < size; i++) {
    term_putchar(s[i]);
  }
}
