/**
 * From professor Charlie Curtsinger
 * src:
 * https://curtsinger.cs.grinnell.edu/teaching/2022S/CSC395/kernel/housekeeping.html
 */
#pragma once

#include <stddef.h>
#include <stdint.h>

#include "kgraphic.h"
#include "kmem.h"
#include "port.h"
#include "psf.h"
#include "stivale2.h"

typedef struct {
  int32_t row;
  int32_t col;
  size_t byte_per_row;
  color_t fg;
  color_t bg;
  bool enable_cursor;
} terminal_t;

// Initialize the terminal
void term_init();

// Reset the color of the terminal
void term_reset_color();

// Set the color of the terminal
void term_set_color(color_t fg, color_t bg);

// Reset the value of terminal object to its initial state
void term_reset();

// Clear the terminal
void term_clear();

// Write one character to the terminal
void term_putchar(char c);

// Write string to the terminal
void term_puts(const char* s, size_t size);