#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "stivale2.h"

#define ARGB32_WHITE 0x00FFFFFF
#define ARGB32_BLACK 0x00000000
#define ARGB32_RED 0x00FF0000
#define ARGB32_GREEN 0x0000FF00
#define ARGB32_BLUE 0x000000FF
#define ARGB32_LIGHT_RED 0x00FF6666
#define ARGB32_LIGHT_GREEN 0x0033FF99
#define ARGB32_LIGHT_BLUE 0x003399FF
#define ARGB32_MAGENTA 0x00FF0FF
#define ARGB32_CYAN 0x0000FFFF
#define ARGB32_YELLOW 0x00FFFF00
#define ARGB32_BROW 0x00CC6600

typedef uint32_t color_t;
typedef uint32_t pixel_t;

/**
 * Read the framebuffer struct tag to gain information about the current
 * framebuffer. The function also initialize psf font. 
 * \returns true if init successfully else returns false.
 */
bool graphic_init();

/**
 * Set the framebuffer value to 0
 */ 
void graphic_clear_buffer();