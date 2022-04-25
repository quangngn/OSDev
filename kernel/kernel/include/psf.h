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


bool psf_init();

bool psf_put_char(char c, size_t row, size_t col, color_t fg, color_t bg,
                  bool enable_cursor);
