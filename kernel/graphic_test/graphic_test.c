#include <graphic.h>
#include <mem.h>
#include <process.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

void _start() {
  window_t window;
  graphic_init_window(&window, 720, 720, 0, 0, ARGB32_LIGHT_GREEN);

  // Set lower half of the window to light blue
  color_t *src = (color_t *)window.addr;
  for (int r = 0; r < window.height / 2; r++) {
    for (int c = 0; c < window.width; c++) {
      src[r * window.width + c] = ARGB32_LIGHT_BLUE;
    }
  }

  graphic_draw(&window, true);

  for (;;) {
  }
}
