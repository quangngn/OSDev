#include <graphic.h>
#include <mem.h>
#include <process.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define DRAW_TIME 100000000
#define INPUT_TIME 100000
#define MOVE_SPEED 10

window_t window;
int32_t window_w = 640;
int32_t window_h = 360;

// Draw a mock-up of a macOS window
void draw_window() {
  rec2d_wh(0, window_h - 1, window_w, 20, ARGB32_GRAY, true, &window);
  rec2d_wh(5, window_h - 5, 10, 10, ARGB32_LIGHT_RED, true, &window);
  rec2d_wh(20, window_h - 5, 10, 10, ARGB32_YELLOW, true, &window);
  rec2d_wh(35, window_h - 5, 10, 10, ARGB32_LIGHT_GREEN, true, &window);
}

void _start() {
  // Init window for user program and draw the mock-up
  window_init(&window, window_w, window_h, 0, 0, ARGB32_LIGHT_BLUE);
  draw_window();

  // Repeat drawing after a certain number of ticks. 
  // The program also get input from keyboard.
  uint64_t prev_draw_time = 0;
  uint64_t prev_input_time = 0;
  while (true) {
    if (get_time() - prev_draw_time > DRAW_TIME) {
      // Draw something
      graphic_clear_screen();
      graphic_draw(&window, false);
      prev_draw_time = get_time();
    }

    // Use the keyboard input to control the cube location on xy-plane
    if (get_time() - prev_input_time > INPUT_TIME) {
      char c = peekc();
      switch (c) {
        case 'a':   // Move the window to the left
          window.screen_x -= MOVE_SPEED;
          break;
        case 'd':   // Move the window to the right
          window.screen_x += MOVE_SPEED;
          break;
        case 'w':   // The y axis of the screen is flipped; origin at top left
          window.screen_y -= MOVE_SPEED;
          break;
        case 's':
          window.screen_y += MOVE_SPEED;
          break;
        case 'f':   // Flipt the window upside down
          window.flip = !window.flip;
          break;
        case 'q':   // Exit and return to shell
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
