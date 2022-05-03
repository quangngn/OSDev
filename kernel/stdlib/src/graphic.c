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

bool graphic_init_window(window_t *window, int width, int height,
                         int screen_x, int screen_y, color_t bg) {
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
bool draw_pixel(const point_t *p, color_t color, window_t *window) {
  if (p == NULL || window == NULL) return false;

  window_set((int)p->x, (int)p->y, color, window);
  return true;
}

bool draw_line(const line_t *l, color_t color, window_t *window) {
  if (l == NULL || window == NULL) return false;

  // Using Bresenham algorithm to print the line
  int x0 = (int)l->p0.x;
  int y0 = (int)l->p0.y;
  int x1 = (int)l->p1.x;
  int y1 = (int)l->p1.x;

  // Get the direction we are computing
  int dx = x1 - x0;
  int dy = y1 - y0;
  int sign_x = get_sign(dx);
  int sign_y = get_sign(dy);
  dx = abs(dx);
  dy = abs(dy);

  // Check for steepness
  bool steep = false;
  if (dx < dy) {
    swap(&dx, &dy);
    steep = true;
  }

  // Compute constant:
  int decision = 2 * dy - dx;
  int step_dec_neg = 2 * dy;
  int step_dec_non_neg = step_dec_neg - 2 * dx;

  // Draw
  window_set(x0, y0, color, window);
  for (int i = 0; i < dx; i++) {
    if (decision < 0) {
      if (steep) {
        y0 += sign_y;
      } else {
        x0 += sign_x;
      }
      decision += step_dec_neg;
    } else {
      x0 += sign_x;
      y0 += sign_y;
      decision += step_dec_non_neg;
    }
    // Draw the point
    window_set(x0, y0, color, window);
  }
  return true;
}