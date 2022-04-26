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

// Number of rows and columns of characters that can be fited within the given
// frame buffer.
size_t term_w = 0;
size_t term_h = 0;

/**
 * Init the information regarding the current psf font.
 * Requirement: The screen pixel width must be a multiple of each glyph's width.
 * Same for height.
 * \returns true if the initialization succeeds and false
 * otherwise.
 */
bool psf_init() {
  psf_t* font = (psf_t*)&_binary_fonts_font_psf_start;
  if (font == NULL) return false;

  // Init global values related to font
  psf_font_w = font->width;
  psf_font_h = font->height;
  psf_glyph_sz = font->bytesperglyph;
  psf_nglyph = font->numglyph;
  psf_glyph_start = (char*)((uintptr_t)font + font->headersize);

  // Init screen information
  term_w = screen_w / psf_font_w;
  term_h = screen_h / psf_font_h;
  return true;
}

/**
 * Using the glyphs table to draw pixel-by-pixel the characters onto the frame
 * framebuffer.
 * \param c: Character to be printed.
 * \param pixel_row: The row index of the top left pixel.
 * \param pixel_col: The col index of the top left pixel.
 * \param fg: Foreground color of the character.
 * \param bg: Background color of the character.
 *
 * \returns true if the printing is within bound, else returns false.
 */
bool psf_put_char(char c, size_t pixel_row, size_t pixel_col, color_t fg,
                  color_t bg) {
  // Check for out of bound
  if (pixel_row >= screen_h - psf_font_h ||
      pixel_col >= screen_w - psf_font_w || c >= psf_nglyph) {
    return false;
  }

  // 1. Find the address of the glymph that corresponds to character c:
  char* glyph = &psf_glyph_start[(uint32_t)c * psf_glyph_sz];

  // 2. Find location on the frame buffer to print:
  pixel_t* row_start = (pixel_t*)buffer_addr + pixel_row * screen_w + pixel_col;

  // 3. Print each row in the glyph to the frame buffer with fg and bg colors:
  for (int i = 0; i < psf_font_h; i++) {
    // cursor points to current pixel in the frame buffer
    pixel_t* cursor = row_start;
    // cur_glyph_row points to the current row in the printed glyph
    uint8_t cur_glyph_row = glyph[i];
    uint8_t mask = 0x80;

    // Each bits in the glyph corresponds to a pixel in the frame buffer.
    // If the bit is 0, we put background color to the frame buffer, else
    // we put foreground color to the frame buffer.
    for (int j = 0; j < psf_font_w; j++) {
      *cursor = ((cur_glyph_row & mask) == 0) ? (pixel_t)bg : (pixel_t)fg;
      mask >>= 1;
      cursor++;
    }

    // Advance to the next row_start in the frame buffer.
    row_start += screen_w;
  }
  return true;
}
