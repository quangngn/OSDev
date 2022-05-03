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

bool graphic_init_window(window_t *window, uint32_t width, uint32_t height,
                         uint32_t screen_x, uint32_t screen_y, color_t bg) {
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

  return true;
}

/******************************************************************************/
bool draw_pixel(const point_t *p, color_t color, window_t *window) {
  if (p == NULL || window == NULL) return false;

  window_set(p->p2d.x, p->p2d.y, color, window);
  return true;
}