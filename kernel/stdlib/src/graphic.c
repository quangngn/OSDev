#include "graphic.h"

// External functions for system call handler. syscall(uint64_t nr, ...) is
// defined in asm/syscall.s
extern int64_t syscall(uint64_t nr, ...);

/******************************************************************************/
// Graphical functions
/**
 * System call to get kernel's framebuffer's information. This information is
 * provided in the framebuffer struct tag.
 */
bool graphic_get_framebuffer_info(framebuffer_info_t *fb_info) {
  if (fb_info == NULL) return false;
  return (bool)syscall(SYSCALL_GET_FRAMEBUFFER_INFO, fb_info);
}

/**
 * Copy the current window's frame buffer to the kernel's framebuffer.
 * \param window Pointer to the window struct.
 * \param flip Boolean whether we flip the buffer vertically. Effectively
 * changing the origin from top-left to bottom-left.
 * \return true if draw successfully.
 */
void graphic_draw(window_t *window, bool flip) {
  if (window == NULL) return;

  // Make draw call
  syscall(SYSCALL_FRAMEBUFFER_CPY, window->addr, window->screen_x,
          window->screen_y, window->width, window->height, flip);
  window_clear(window);
}

/******************************************************************************/
// Window functions
/**
 * Init the window struct to hold information about the user's writable buffer.
 * \param window Pointer to the window struct.
 * \param width Width of the window (in pixel).
 * \param height Height of the window (in pixel).
 * \param screen_x Horizontal pixel coordinate that this window being drawn to
 * the screen (aka kernel's buffer).
 * \param screen_x Vertical pixel coordinate that this window being drawn to
 * the screen (aka kernel's buffer).
 * \param bg Default background color.
 * \return true if succeed.
 */
bool window_init(window_t *window, int width, int height, int screen_x,
                 int screen_y, color_t bg) {
  if (window == NULL) return false;

  // Memmap address for the window's framebuffer. This would start at
  // USER_FRAMEBUFFER defined in system.h.
  if (mmap((void *)USER_FRAMEBUFFER, width * height * sizeof(pixel_t),
           (PROT_READ | PROT_WRITE), 0, 0, 0) == NULL) {
    return false;
  }

  window->addr = (pixel_t *)USER_FRAMEBUFFER;
  window->width = width;
  window->height = height;
  window->screen_x = screen_x;
  window->screen_y = screen_y;
  window->bg = bg;
  // Set buffer to the default background color. This would also set dirty_buff
  // to false
  window_clear(window);

  return true;
}

/**
 * Clear the window's framebuffer by setting to the default color.
 * \param window Pointer to the window struct.
 */
void window_clear(window_t *window) {
  if (window == NULL) return;

  color_t *src = (color_t *)window->addr;
  for (int r = 0; r < window->height; r++) {
    for (int c = 0; c < window->width; c++) {
      src[r * window->width + c] = window->bg;
    }
  }
}

/******************************************************************************/
/**
 * Draw pixel onto the window's buffer. By default, the window have origin
 * coordinate (0, 0) at the top left corner.
 * \param x The horizontal coordinate of the pixel.
 * \param y The vertical coordinate of the pixel.
 * \param color The color of the pixel.
 * \param window Pointer to the target window.
 * \return true if draw succeeds.
 */
bool draw_pixel(int x, int y, color_t color, window_t *window) {
  // Check if pixel is out of bound
  if (window == NULL || x >= window->width || x < 0 || y >= window->height ||
      y < 0) {
    return false;
  } else {
    // Draw the pixel to the window
    (window->addr)[x + y * window->width] = color;
    return true;
  }
}

/**
 * Draw pixel onto the window's buffer. By default, the window have origin
 * coordinate (0, 0) at the top left corner.
 * \param p Pointer to the point struct.
 * \param color The color of the pixel.
 * \param window Pointer to the target window.
 * \return true if draw succeeds.
 */
bool draw_pixel_p(point_t *p, color_t color, window_t *window) {
  return draw_pixel((int)p->x, (int)p->y, color, window);
}

/**
 * Credit: Dmitry V. Sokolov
 * Link:
 * https://github.com/ssloy/tinyrenderer/wiki/Lesson-1:-Bresenham’s-Line-Drawing-Algorithm
 *
 * Draw line onto the window's buffer. By default, the window have origin
 * coordinate (0, 0) at the top left corner.
 * \param x0 The horizontal coordinate of the start point.
 * \param y0 The vertical coordinate of the start point.
 * \param x1 The horizontal coordinate of the end point.
 * \param y1 The vertical coordinate of the end point.
 * \param color The color of the line.
 * \param window Pointer to the target window.
 * \return true if draw succeeds.
 */
bool draw_line(int x0, int y0, int x1, int y1, color_t color,
               window_t *window) {
  if (window == NULL) return false;

  // Using Bresenham algorithm to print the line
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
      draw_pixel(y, x, color, window);
      error2 += derror2;
      if (error2 > dx) {
        y += (y1 > y0 ? 1 : -1);
        error2 -= dx * 2;
      }
    }
  } else {
    for (int x = x0; x <= x1; x++) {
      draw_pixel(x, y, color, window);
      error2 += derror2;
      if (error2 > dx) {
        y += (y1 > y0 ? 1 : -1);
        error2 -= dx * 2;
      }
    }
  }
  return true;
}

/**
 * Draw line onto the window's buffer. By default, the window have origin
 * coordinate (0, 0) at the top left corner.
 * \param l Pointer to the line object.
 * \param color The color of the line.
 * \param window Pointer to the target window.
 * \return true if draw succeeds.
 */
bool draw_line_l(const line_t *l, color_t color, window_t *window) {
  if (l == NULL || window == NULL) return false;

  return draw_line((int)l->p0.x, (int)l->p0.y, (int)l->p1.x, (int)l->p1.y,
                   color, window);
}

/**
 * Draw a triangle onto the window's buffer. By default, the window have origin
 * coordinate (0, 0) at the top left corner.
 * \param x0 The horizontal coordinate of the 1st vertex.
 * \param y0 The vertical coordinate of the 1st vertex.
 * \param x1 The horizontal coordinate of the 2nd vertex.
 * \param y1 The vertical coordinate of the 2nd vertex.
 * \param x2 The horizontal coordinate of the 3rd vertex.
 * \param y2 The vertical coordinate of the 3rd vertex.
 * \param color The color of the triangle.
 * \param fill Boolean whether the shape is filled.
 * \param window Pointer to the target window.
 * \return true if draw succeeds.
 */
bool draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2,
                   color_t color, bool fill, window_t *window) {
  if (window == NULL) return false;

  int x_min = max(min(x0, min(x1, x2)), 0);
  int x_max = min(max(x0, max(x1, x2)), window->width - 1);
  int y_min = max(min(y0, min(y1, y2)), 0);
  int y_max = min(max(y0, max(y1, y2)), window->height - 1);

  if (fill) {
    for (int y = y_min; y <= y_max; y++) {
      for (int x = x_min; x <= x_max; x++) {
        int z1 = det2(x - x0, y - y0, x1 - x0, y1 - y0);
        int z2 = det2(x - x1, y - y1, x2 - x1, y2 - y1);
        int z3 = det2(x - x2, y - y2, x0 - x2, y0 - y2);

        if ((z1 >= 0 && z2 >= 0 && z3 >= 0) ||
            (z1 <= 0 && z2 <= 0 && z3 <= 0)) {
          draw_pixel(x, y, color, window);
        }
      }
    }
    return true;
  } else {
    return draw_line(x0, y0, x1, y1, color, window) &&
           draw_line(x1, y1, x2, y2, color, window) &&
           draw_line(x2, y2, x0, y0, color, window);
  }
}

/**
 * Draw a triangle onto the window's buffer. By default, the window have origin
 * coordinate (0, 0) at the top left corner.
 * \param t Pointer to the triangle.
 * \param color The color of the triangle.
 * \param fill Boolean whether the shape is filled.
 * \param window Pointer to the target window.
 * \return true if draw succeeds.
 */
bool draw_triangle_t(const triangle_t *t, color_t color, bool fill,
                     window_t *window) {
  if (t == NULL || window == NULL) return false;

  return draw_triangle((int)t->p0.x, (int)t->p0.y, (int)t->p1.x, (int)t->p1.y,
                       (int)t->p2.x, (int)t->p2.y, color, fill, window);
}