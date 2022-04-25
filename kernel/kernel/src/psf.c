#include "psf.h"

// Struct tags defined in boot.c
extern struct stivale2_struct_tag_framebuffer* framebuffer_struct_tag;

// Defined from kernel/obj/fonts/font.o
extern char _binary_fonts_font_psf_start;
extern char _binary_fonts_font_psf_end;

// Defined in graphic.c
extern size_t screen_w;
extern size_t screen_h;
extern uintptr_t buffer_addr;

// Global variables related to the loaded psf font
uint32_t psf_font_w = 0;
uint32_t psf_font_h = 0;
uint32_t psf_glyph_sz = 0;
uint32_t psf_nglyph = 0;
char* psf_glyph_start = NULL;
char* psf_glyph_end = NULL;

size_t term_w = 0;
size_t term_h = 0;

// Requirement for now, the
bool psf_init() {
  // Init global values related to font
  psf_t* font = (psf_t*)&_binary_fonts_font_psf_start;
  psf_font_w = font->width;
  psf_font_h = font->height;
  psf_glyph_sz = font->bytesperglyph;
  psf_nglyph = font->numglyph;
  psf_glyph_start = (uintptr_t)font + font->headersize;

  // Init screen information
  term_w = screen_w / psf_font_w;
  term_h = screen_h / psf_font_h;
  return true;
}

bool psf_put_char(char c, size_t row, size_t col, color_t fg, color_t bg,
                  bool enable_cursor) {
  if (row >= term_h || col >= term_w || c >= psf_nglyph) {
    return false;
  }

  // 1. Find the address of the glymph that corresponds to character c:
  char* glyph = psf_glyph_start[(uint32_t)c * psf_glyph_sz];

  // 2. Find location on the frame buffer to print:
  size_t pixel_row = row * psf_font_h;
  size_t pixel_col = col * psf_font_w;
  pixel_t* row_start = (pixel_t*)buffer_addr + pixel_row * screen_w + pixel_col;

  // 3. Print each row in the glyph to the frame buffer with fg and bg colors:
  for (int i = 0; i < psf_font_h; i++) {
    // cursor points to current pixel in the frame buffer
    pixel_t* cursor = row_start;
    // cur_glyph_row points to the current row in the printed glyph
    uint8_t cur_glyph_row = glyph[i];

    // Each bits in the glyph corresponds to a pixel in the frame buffer.
    // If the bit is 0, we put background color to the frame buffer, else
    // we put foreground color to the frame buffer.
    for (int j = 0; j < psf_font_w; j++) {
      *cursor = cur_glyph_row % 2 == 0 ? (pixel_t)bg : (pixel_t)fg;
      cur_glyph_row /= 2;
      cursor++;
    }

    // Advance to the next row_start in the frame buffer.
    row_start += screen_w;
  }
  return true;
}
