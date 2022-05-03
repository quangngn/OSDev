#include "graphic.h"

// External functions for system call handler. syscall(uint64_t nr, ...) is
// defined in asm/syscall.s
extern int64_t syscall(uint64_t nr, ...);

// System function to setup user's framebuffer for app's window
bool graphic_get_framebuffer_info(framebuffer_info_t *fb_info) {
  if (fb_info == NULL) return false;
  return (bool)syscall(SYSCALL_GET_FRAMEBUFFER_INFO, fb_info);
}

bool graphic_draw(window_t *window) {
  if (window == NULL) return false;
  return (bool)syscall(SYSCALL_FRAMEBUFFER_CPY, window->addr, window->screen_x,
                       window->screen_y, window->width, window->height);
}

