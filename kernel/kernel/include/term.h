#pragma once

#include <stddef.h>
#include <stdint.h>

#include "port.h"
#include "stivale2.h"
#include "util.h"

#define VGA_BUFFER 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define VGA_COLOR_BLACK 0
#define VGA_COLOR_BLUE 1
#define VGA_COLOR_GREEN 2
#define VGA_COLOR_CYAN 3
#define VGA_COLOR_RED 4
#define VGA_COLOR_MAGENTA 5
#define VGA_COLOR_BROWN 6
#define VGA_COLOR_LIGHT_GREY 7
#define VGA_COLOR_DARK_GREY 8
#define VGA_COLOR_LIGHT_BLUE 9
#define VGA_COLOR_LIGHT_GREEN 10
#define VGA_COLOR_LIGHT_CYAN 11
#define VGA_COLOR_LIGHT_RED 12
#define VGA_COLOR_LIGHT_MAGENTA 13
#define VGA_COLOR_LIGHT_BROWN 14
#define VGA_COLOR_WHITE 15

// Struct representing a single character entry in the VGA buffer
typedef struct vga_entry {
  uint8_t c;
  uint8_t fg : 4;
  uint8_t bg : 4;
} __attribute__((packed)) vga_entry_t;

// Turn on the VGA cursor
void term_enable_cursor();

// Update the VGA cursor
void term_update_cursor();

// Clear the terminal
void term_clear();

// Write one character to the terminal
void term_putchar(char c, uint8_t fg, uint8_t bg);

// Write string to the terminal
void term_puts(const char* s, size_t size, uint8_t fg, uint8_t bg);

// Initialize the terminal
void term_init();