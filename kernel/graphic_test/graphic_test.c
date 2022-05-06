#include <graphic.h>
#include <mem.h>
#include <process.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

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

  draw_line_l(&l0, ARGB32_RED, &window);
  draw_line_l(&l1, ARGB32_RED, &window);
  draw_line_l(&l2, ARGB32_RED, &window);
  draw_line_l(&l3, ARGB32_RED, &window);
  draw_line_l(&l4, ARGB32_RED, &window);
  draw_line_l(&l5, ARGB32_RED, &window);
  draw_line_l(&l6, ARGB32_RED, &window);
  draw_line_l(&l7, ARGB32_RED, &window);
}

void draw_triangle_pattern() {
  point_t p0 = {.x = 360, .y = 360, .z = 0, .w = 0};

  point_t p1 = {.x = 0, .y = 720, .z = 0, .w = 0};
  point_t p2 = {.x = 360, .y = 720, .z = 0, .w = 0};
  point_t p3 = {.x = 720, .y = 720, .z = 0, .w = 0};

  point_t p4 = {.x = 0, .y = 360, .z = 0, .w = 0};
  point_t p5 = {.x = 360, .y = 360, .z = 0, .w = 0};
  point_t p6 = {.x = 720, .y = 360, .z = 0, .w = 0};

  point_t p7 = {.x = 0, .y = 0, .z = 0, .w = 0};
  point_t p8 = {.x = 360, .y = 0, .z = 0, .w = 0};
  point_t p9 = {.x = 720, .y = 0, .z = 0, .w = 0};

  triangle_t t0 = {.p0 = p0, .p1 = p1, .p2 = p2};
  triangle_t t1 = {.p0 = p0, .p1 = p2, .p2 = p3};
  triangle_t t2 = {.p0 = p0, .p1 = p3, .p2 = p6};
  triangle_t t3 = {.p0 = p0, .p1 = p6, .p2 = p9};
  triangle_t t4 = {.p0 = p0, .p1 = p9, .p2 = p8};
  triangle_t t5 = {.p0 = p0, .p1 = p8, .p2 = p7};
  triangle_t t6 = {.p0 = p0, .p1 = p7, .p2 = p4};
  triangle_t t7 = {.p0 = p0, .p1 = p4, .p2 = p1};

  draw_triangle_t(&t0, ARGB32_RED, true, &window);
  draw_triangle_t(&t1, ARGB32_BLUE, true, &window);
  draw_triangle_t(&t2, ARGB32_GREEN, true, &window);
  draw_triangle_t(&t3, ARGB32_LIGHT_RED, true, &window);
  draw_triangle_t(&t4, ARGB32_LIGHT_BLUE, true, &window);
  draw_triangle_t(&t5, ARGB32_LIGHT_GREEN, true, &window);
  draw_triangle_t(&t6, ARGB32_BROW, true, &window);
  draw_triangle_t(&t7, ARGB32_CYAN, true, &window);
}

void running_triangle() {
  triangle_t t = {.p0 = {.x = 0, .y = 0, .z = 0, .w = 0},
                  .p1 = {.x = 0, .y = 20, .z = 0, .w = 0},
                  .p2 = {.x = 20, .y = 0, .z = 0, .w = 0}};

  uint64_t prev_time = get_time();
  draw_triangle_t(&t, ARGB32_YELLOW, true, &window);
  while (t.p0.x < 720) {
    if (get_time() - prev_time > 10000000) {
      // Draw
      draw_triangle_t(&t, ARGB32_YELLOW, true, &window);
      graphic_draw(&window, true);

      // Update object
      t.p0.x = min(720, t.p0.x + 1);
      t.p0.y = min(720, t.p0.y + 1);
      t.p1.x = min(720, t.p1.x + 1);
      t.p1.y = min(720, t.p1.y + 1);
      t.p2.x = min(720, t.p2.x + 1);
      t.p2.y = min(720, t.p2.y + 1);

      // Update time
      prev_time = get_time();
    }
  }
}

draw_test_rectangle() {
  point_t p0 = {.x = 0, .y = 320, .z = 0, .w = 0};

  point_t p1 = {.x = 320, .y = 320, .z = 0, .w = 0};
  point_t p2 = {.x = 0, .y = 0, .z = 0, .w = 0};
  point_t p3 = {.x = 320, .y = 0, .z = 0, .w = 0};

  rectangle_t r0 = {.p0 = p0, .p1 = p1, .p2 = p2, .p3 = p3};
  draw_rectangle_r(&r0, ARGB32_RED, true, &window);
}

void _start() {
  window_init(&window, 720, 720, 0, 0, ARGB32_BLUE);
  // draw_triangle_pattern();
  //  running_triangle();

  draw_test_rectangle();
  graphic_draw(&window, true);

  // running_triangle();

  for (;;) {
  }
}