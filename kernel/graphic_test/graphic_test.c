#include <graphic.h>
#include <mem.h>
#include <process.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

window_t window;

void draw_star() {
  line_t l0 = {.p0 = {.x = 360, .y = 360}, .p1 = {.x = 720, .y = 360}};
  line_t l1 = {.p0 = {.x = 360, .y = 360}, .p1 = {.x = 720, .y = 720}};
  line_t l2 = {.p0 = {.x = 360, .y = 360}, .p1 = {.x = 360, .y = 720}};
  line_t l3 = {.p0 = {.x = 360, .y = 360}, .p1 = {.x = 0, .y = 720}};
  line_t l4 = {.p0 = {.x = 360, .y = 360}, .p1 = {.x = 0, .y = 360}};
  line_t l5 = {.p0 = {.x = 360, .y = 360}, .p1 = {.x = 0, .y = 0}};
  line_t l6 = {.p0 = {.x = 360, .y = 360}, .p1 = {.x = 360, .y = 0}};
  line_t l7 = {.p0 = {.x = 360, .y = 360}, .p1 = {.x = 720, .y = 0}};

  draw_line(&l0, ARGB32_RED, &window);
  draw_line(&l1, ARGB32_RED, &window);
  draw_line(&l2, ARGB32_RED, &window);
  draw_line(&l3, ARGB32_RED, &window);
  draw_line(&l4, ARGB32_RED, &window);
  draw_line(&l5, ARGB32_RED, &window);
  draw_line(&l6, ARGB32_RED, &window);
  draw_line(&l7, ARGB32_RED, &window);
}

void _start() {
  graphic_init_window(&window, 720, 720, 0, 0, ARGB32_LIGHT_GREEN);

  draw_star();

  graphic_draw(&window, true);

  for (;;) {
  }
}
