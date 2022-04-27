/**
 * From professor Charlie Curtsinger
 * src:
 * https://curtsinger.cs.grinnell.edu/teaching/2022S/CSC395/kernel/housekeeping.html
 */
#include "term.h"

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
void term_init() {
  // Init terminal's state values and clear buffer
  term_reset();
  term_clear();
}

// Reset the color of the terminal to white and black
void term_reset_color() {
  term.fg = ARGB32_WHITE;
  term.bg = ARGB32_BLACK;
}

// Set the color of the terminal
void term_set_color(color_t new_fg, color_t new_bg) {
  term.fg = new_fg;
  term.bg = new_bg;
}

// Reset the value of terminal object to its initial state
void term_reset() {
  term.row = 0;
  term.col = 0;
  term.fg = ARGB32_WHITE;
  term.bg = ARGB32_BLACK;
  term.enable_cursor = true;
  // Number of byte in frame buffer buffer for each row of the terminal
  term.byte_per_row = screen_w * psf_font_h * sizeof(pixel_t);
}

// Clear the terminal
void term_clear() {
  // Clear the framebuffer
  graphic_clear_buffer();
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
  } else {
    term_put_psf_char(c);
    term.col++;
  }

  // Make sure the cursor is in the writable location
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
    // Set cursor to the start of the bottom line
    term.row--;

    // Clear the last row
    uintptr_t last_row = buffer_addr + copy_size;
    kmemset((void*)last_row, 0, term.byte_per_row);
  }
}

// Write string to the terminal
void term_puts(const char* s, size_t size) {
  for (int i = 0; i < size; i++) {
    term_putchar(s[i]);
  }
}
