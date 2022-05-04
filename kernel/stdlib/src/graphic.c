#include "graphic.h"

// External functions for system call handler. syscall(uint64_t nr, ...) is
// defined in asm/syscall.s
extern int64_t syscall(uint64_t nr, ...);

// System function to setup user's framebuffer for app's window
bool graphic_get_framebuffer_info(framebuffer_info_t *fb_info) {
  if (fb_info == NULL) return false;
  return (bool)syscall(SYSCALL_GET_FRAMEBUFFER_INFO, fb_info);
}

bool graphic_draw(window_t *window, bool flip) {
  if (window == NULL) return false;
  return (bool)syscall(SYSCALL_FRAMEBUFFER_CPY, window->addr, window->screen_x,
                       window->screen_y, window->width, window->height, flip);
}

bool graphic_clear_window(window_t *window) {
  if (window == NULL) return NULL;
  color_t *src = (color_t *)window->addr;

  for (int r = 0; r < window->height; r++) {
    for (int c = 0; c < window->width; c++) {
      src[r * window->width + c] = window->bg;
    }
  }
  return true;
}

bool graphic_init_window(window_t *window, int width, int height, int screen_x,
                         int screen_y, color_t bg) {
  if (window == NULL) return false;

  // Memmap address for the window's framebuffer. This would start at
  // USER_FRAMEBUFFER defined in system.h
  if (mmap((void *)USER_FRAMEBUFFER, width * height * sizeof(pixel_t),
           (PROT_READ | PROT_WRITE), 0, 0, 0) == NULL)
    return false;

  window->addr = (pixel_t *)USER_FRAMEBUFFER;
  window->width = width;
  window->height = height;
  window->screen_x = screen_x;
  window->screen_y = screen_y;
  window->bg = bg;

  // Set the value of current window's buffer to bg color
  graphic_clear_window(window);

  return true;
}

/******************************************************************************/
void window_set(int x, int y, color_t color, window_t *window) {
  // Check if pixel is out of bound
  if (window == NULL || x >= window->width || x < 0 || y >= window->height ||
      y < 0) {
    return;
  }

  // Draw the pixel to the window
  window->addr[x + y * window->width] = color;
}

bool draw_pixel(const point_t *p, color_t color, window_t *window) {
  if (p == NULL || window == NULL) return false;

  window_set((int)p->x, (int)p->y, color, window);
  return true;
}

/**
 * Credit: Dmitry V. Sokolov
 * Link:
 * https://github.com/ssloy/tinyrenderer/wiki/Lesson-1:-Bresenham’s-Line-Drawing-Algorithm
 */
bool draw_line(const line_t *l, color_t color, window_t *window) {
  if (l == NULL || window == NULL) return false;

  // Using Bresenham algorithm to print the line
  int x0 = (int)l->p0.x;
  int y0 = (int)l->p0.y;
  int x1 = (int)l->p1.x;
  int y1 = (int)l->p1.y;

  bool steep = false;
  if (abs(x0 - x1) < abs(y0 - y1)) {
    swap(&x0, &y0);
    swap(&x1, &y1);
    steep = true;
  }
  if (x0 > x1) {
    swap(&x0, &x1);
    swap(&y0, &y1);
  }
  int dx = x1 - x0;
  int dy = y1 - y0;
  int derror2 = abs(dy) * 2;
  int error2 = 0;
  int y = y0;
  if (steep) {
    for (int x = x0; x <= x1; x++) {
      window_set(y, x, color, window);
      error2 += derror2;
      if (error2 > dx) {
        y += (y1 > y0 ? 1 : -1);
        error2 -= dx * 2;
      }
    }
  } else {
    for (int x = x0; x <= x1; x++) {
      window_set(x, y, color, window);
      error2 += derror2;
      if (error2 > dx) {
        y += (y1 > y0 ? 1 : -1);
        error2 -= dx * 2;
      }
    }
  }
  return true;
}