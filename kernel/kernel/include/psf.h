#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "kgraphic.h"
#include "stivale2.h"

/**
 * Src: https://wiki.osdev.org/PC_Screen_Font
 */
#define PSF_FONT_MAGIC 0x864ab572

typedef struct psf {
  uint32_t magic;         /* magic bytes to identify PSF */
  uint32_t version;       /* zero */
  uint32_t headersize;    /* offset of bitmaps in file, 32 */
  uint32_t flags;         /* 0 if there's no unicode table */
  uint32_t numglyph;      /* number of glyphs */
  uint32_t bytesperglyph; /* size of each glyph */
  uint32_t height;        /* height in pixels */
  uint32_t width;         /* width in pixels */
} psf_t;

/**
 * Init the information regarding the current psf font.
 * Requirement: The screen pixel width must be a multiple of each glyph's width.
 * Same for height.
 * \returns true if the initialization succeeds and false
 * otherwise.
 */
bool psf_init();

/**
 * Using the glyphs table to draw pixel-by-pixel the characters onto the frame
 * framebuffer.
 * \param c: Character to be printed.
 * \param pixel_row: The row index of the top left pixel.
 * \param pixel_col: The col index of the top left pixel.
 * \param fg: Foreground color of the character.
 * \param bg: Background color of the character.
 * \returns true if the printing is within bound, else returns false.
 */
bool psf_put_char(char c, size_t pixel_row, size_t pixel_col, color_t fg,
                  color_t bg);
