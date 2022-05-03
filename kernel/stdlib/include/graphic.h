#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "math.h"

typedef struct {
  uintptr_t addr;
  uint32_t width;
  uint32_t height;
  uint32_t byte_per_pixel;
  uint32_t screen_x;
  uint32_t screen_y;
} window_t;

typedef struct {
  uint16_t framebuffer_width;  // Width and height in pixels
  uint16_t framebuffer_height;
  uint16_t framebuffer_pitch;  // Pitch in bytes
  uint16_t framebuffer_bpp;    // Bits per pixel
  uint8_t memory_model;   // Memory model: 1=RGB, all other values undefined
  uint8_t red_mask_size;  // RGB mask sizes and left shifts
  uint8_t red_mask_shift;
  uint8_t green_mask_size;
  uint8_t green_mask_shift;
  uint8_t blue_mask_size;
  uint8_t blue_mask_shift;
  uint8_t unused;
} framebuffer_info_t;

// System function to setup user's framebuffer for app's window
bool graphic_get_framebuffer_info(framebuffer_info_t *fb_info);
bool draw(window_t *window_t);

// Draw primitive to user's framebuffer
bool draw_pixel(int x, int y, color_t color, bool flip);
bool draw_pixel(int x, int y, color_t color, bool flip);

