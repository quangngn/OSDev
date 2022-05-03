#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "math.h"
#include "mem.h"

typedef struct {
  pixel_t* addr;
  color_t bg;
  uint32_t width;
  uint32_t height;
  int32_t screen_x;
  int32_t screen_y;
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

typedef union {
  fvec2_t p2d;
  fvec4_t p4d;
} point_t;

typedef struct {
  point_t p0;
  point_t p1;
} line_t;

typedef struct {
  line_t l0;
  line_t l1;
  line_t l2;
} triangle_t;

// System function to setup user's framebuffer for app's window
bool graphic_get_framebuffer_info(framebuffer_info_t* fb_info);
bool graphic_init_window(window_t* window, uint32_t width, uint32_t height,
                         uint32_t screen_x, uint32_t screen_y, color_t bg);
bool draw(window_t* window_t, bool flip);

// Draw primitive to user's framebuffer in 2D
static inline void window_set(int x, int y, color_t color, window_t* window) {
  window->addr[x + y * window->width] = color;
}
bool draw_pixel(const point_t* p, color_t color, window_t* window);
bool draw_line(const line_t* l, color_t color, window_t* window_t);
bool draw_triangle(const triangle_t* t, color_t, bool fill, window_t* window_t);
