#include <graphic.h>
#include <mem.h>
#include <process.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <system.h>
#include <time.h>

#define DRAW_TIME 1000000
#define INPUT_TIME 100000
#define MOVE_SPEED 5

window_t window;
int32_t window_w = 960;
int32_t window_h = 720;

object_t make_color_cube(float w, float h, float d, int x, int y, int z) {
  object_t cube;

  float half_w = w / 2;
  float half_h = h / 2;
  float half_d = d / 2;

  // Translation variable
  cube.dx = x;
  cube.dy = y;
  cube.dz = z;

  cube.sx = 1.0;
  cube.sy = 1.0;
  cube.sz = 1.0;

  cube.rot_angle = 0;
  cube.rot_axis = (fvec4_t){.x = 1 / 1.4142, .y = 1 / 1.4142, .z = 0, .w = 0};
  // cube.rot_axis = x_axis;

  cube.nb_tri = 12;
  // Vertices insides the cube
  point_t p0 = {.x = -half_w, .y = -half_h, .z = half_d, .w = 1};
  point_t p1 = {.x = -half_w, .y = half_h, .z = half_d, .w = 1};
  point_t p2 = {.x = half_w, .y = half_h, .z = half_d, .w = 1};
  point_t p3 = {.x = half_w, .y = -half_h, .z = half_d, .w = 1};

  point_t p4 = {.x = -half_w, .y = -half_h, .z = -half_d, .w = 1};
  point_t p5 = {.x = -half_w, .y = half_h, .z = -half_d, .w = 1};
  point_t p6 = {.x = half_w, .y = half_h, .z = -half_d, .w = 1};
  point_t p7 = {.x = half_w, .y = -half_h, .z = -half_d, .w = 1};

  // Facet 1: Front
  triangle_t *mesh = (triangle_t *)malloc(sizeof(triangle_t) * 12);
  color_t *colors = (color_t *)malloc(sizeof(color_t) * 12);
  mesh[0] = (triangle_t){.p0 = p0, .p1 = p1, .p2 = p3};
  mesh[1] = (triangle_t){.p0 = p2, .p1 = p1, .p2 = p3};
  colors[0] = ARGB32_RED;
  colors[1] = ARGB32_RED;

  // Facet 2: Back
  mesh[2] = (triangle_t){.p0 = p4, .p1 = p5, .p2 = p7};
  mesh[3] = (triangle_t){.p0 = p6, .p1 = p5, .p2 = p7};
  colors[2] = ARGB32_BLUE;
  colors[3] = ARGB32_BLUE;

  // Facet 3: Top
  mesh[4] = (triangle_t){.p0 = p1, .p1 = p5, .p2 = p2};
  mesh[5] = (triangle_t){.p0 = p6, .p1 = p5, .p2 = p2};
  colors[4] = ARGB32_GREEN;
  colors[5] = ARGB32_GREEN;

  // Facet 4: Bottom
  mesh[6] = (triangle_t){.p0 = p0, .p1 = p4, .p2 = p3};
  mesh[7] = (triangle_t){.p0 = p7, .p1 = p4, .p2 = p3};
  colors[6] = ARGB32_MAGENTA;
  colors[7] = ARGB32_MAGENTA;

  // Facet 5: Right
  mesh[8] = (triangle_t){.p0 = p3, .p1 = p2, .p2 = p7};
  mesh[9] = (triangle_t){.p0 = p6, .p1 = p2, .p2 = p7};
  colors[8] = ARGB32_YELLOW;
  colors[9] = ARGB32_YELLOW;

  // Facet 6: Left
  mesh[10] = (triangle_t){.p0 = p0, .p1 = p1, .p2 = p4};
  mesh[11] = (triangle_t){.p0 = p5, .p1 = p1, .p2 = p4};
  colors[10] = ARGB32_CYAN;
  colors[11] = ARGB32_CYAN;

  cube.mesh = mesh;
  cube.mess_color = colors;

  return cube;
}

void _start() {
  // Init window to draw
  window_init(&window, window_w, window_h, 0, 0, ARGB32_GRAY);

  // Create a cube
  object_t cube =
      make_color_cube(50, 50, 50, window_w / 2 + 100, window_h / 2, 0);
  object_t cube2 =
      make_color_cube(50, 50, 50, window_w / 2 - 100, window_h / 2, 0);

  // Draw the cube for the first time;
  obj3d_o(&cube, true, true, false, false, &window);
  graphic_draw(&window, true);

  // Repeat drawing after a certain number of ticks. The program also get input
  // from keyboard.
  uint64_t prev_draw_time = 0;
  uint64_t prev_input_time = 0;
  while (true) {
    if (get_time() - prev_draw_time > DRAW_TIME) {
      cube.rot_angle += 1;
      cube2.rot_angle += 1;
      obj3d_o(&cube, true, true, false, false, &window);
      obj3d_o(&cube2, true, true, false, true, &window);
      graphic_draw(&window, true);
      prev_draw_time = get_time();
    }

    // Use the keyboard input to control the cube location on xy-plane
    if (get_time() - prev_input_time > INPUT_TIME) {
      char c = peekc();
      switch (c) {
        case 'a':
          cube.dx -= MOVE_SPEED;
          cube2.dx -= MOVE_SPEED;
          break;
        case 'd':
          cube.dx += MOVE_SPEED;
          cube2.dx += MOVE_SPEED;
          break;
        case 'w':
          cube.dy += MOVE_SPEED;
          cube2.dy += MOVE_SPEED;
          break;
        case 's':
          cube.dy -= MOVE_SPEED;
          cube2.dy -= MOVE_SPEED;
          break;
        case 'q':
          exit();
        default:
          break;
      }
      prev_input_time = get_time();
    }
  }

  for (;;) {
  }
}
