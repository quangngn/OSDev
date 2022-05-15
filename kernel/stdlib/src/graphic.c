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
 * \return true if draw successfully.
 */
void graphic_draw(window_t *window, bool clear) {
  if (window == NULL) return;

  // Make draw call
  syscall(SYSCALL_FRAMEBUFFER_CPY, window->addr, (int64_t)window->screen_x,
          (int64_t)window->screen_y, (int64_t)window->width,
          (int64_t)window->height, (int64_t)window->flip);

  // Clear the window buffer if needed
  if (clear) window_clear(window);
}

/**
 * Clear the kernel's buffer
 */
void graphic_clear_screen() { syscall(SYSCALL_FRAMEBUFFER_CLEAR); }

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
  if (mmap((void *)USER_FRAMEBUFFER, 2 * width * height * sizeof(pixel_t),
           (PROT_READ | PROT_WRITE), 0, 0, 0) == NULL) {
    return false;
  }

  window->addr = (pixel_t *)USER_FRAMEBUFFER;
  window->z_buffer = (int32_t *)(window->addr + width * height);
  window->width = width;
  window->height = height;
  window->screen_x = screen_x;
  window->screen_y = screen_y;
  window->bg = bg;
  window->flip = true; // default for bottom-left origin.
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
  int32_t *z_buff = window->z_buffer;
  for (int r = 0; r < window->height; r++) {
    for (int c = 0; c < window->width; c++) {
      src[r * window->width + c] = window->bg;
      z_buff[r * window->width + c] = INT32_MIN;
    }
  }
}

/******************************************************************************/
/**
 * Draw pixel onto the window's buffer. By default, the window have origin
 * coordinate (0, 0) at the bottom left corner.
 * \param x The horizontal coordinate of the pixel.
 * \param y The vertical coordinate of the pixel.
 * \param color The color of the pixel.
 * \param window Pointer to the target window.
 * \return true if draw succeeds.
 */
bool pixel2d(int x, int y, color_t color, window_t *window) {
  // Check if pixel is out of bound
  if (window == NULL) {
    return false;
  } else {
    if (x < window->width && x >= 0 && y < window->height && y >= 0) {
      // Draw the pixel to the window
      (window->addr)[x + y * window->width] = color;
    }
    return true;
  }
}

/**
 * Draw pixel onto the window's buffer. By default, the window have origin
 * coordinate (0, 0) at the bottom left corner.
 * \param p Pointer to the point struct.
 * \param color The color of the pixel.
 * \param window Pointer to the target window.
 * \return true if draw succeeds.
 */
bool pixel2d_p(point_t *p, color_t color, window_t *window) {
  return pixel2d((int)p->x, (int)p->y, color, window);
}

/**
 * Credit: Dmitry V. Sokolov
 * Link:
 * https://github.com/ssloy/tinyrenderer/wiki/Lesson-1:-Bresenham’s-Line-Drawing-Algorithm
 *
 * Draw line onto the window's buffer. By default, the window have origin
 * coordinate (0, 0) at the bottom left corner.
 * \param x0 The horizontal coordinate of the start point.
 * \param y0 The vertical coordinate of the start point.
 * \param x1 The horizontal coordinate of the end point.
 * \param y1 The vertical coordinate of the end point.
 * \param color The color of the line.
 * \param window Pointer to the target window.
 * \return true if draw succeeds.
 */
bool line2d(int x0, int y0, int x1, int y1, color_t color, window_t *window) {
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
      pixel2d(y, x, color, window);
      error2 += derror2;
      if (error2 > dx) {
        y += (y1 > y0 ? 1 : -1);
        error2 -= dx * 2;
      }
    }
  } else {
    for (int x = x0; x <= x1; x++) {
      pixel2d(x, y, color, window);
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
 * coordinate (0, 0) at the bottom left corner.
 * \param l Pointer to the line object.
 * \param color The color of the line.
 * \param window Pointer to the target window.
 * \return true if draw succeeds.
 */
bool line2d_l(const line_t *l, color_t color, window_t *window) {
  if (l == NULL || window == NULL) return false;

  return line2d((int)l->p0.x, (int)l->p0.y, (int)l->p1.x, (int)l->p1.y, color,
                window);
}

/**
 * Draw a triangle onto the window's buffer. By default, the window have origin
 * coordinate (0, 0) at the bottom left corner.
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
bool tri2d(int x0, int y0, int x1, int y1, int x2, int y2, color_t color,
           bool fill, window_t *window) {
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
          pixel2d(x, y, color, window);
        }
      }
    }
    return true;
  } else {
    return line2d(x0, y0, x1, y1, color, window) &&
           line2d(x1, y1, x2, y2, color, window) &&
           line2d(x2, y2, x0, y0, color, window);
  }
}

/**
 * Draw a triangle onto the window's buffer. By default, the window have origin
 * coordinate (0, 0) at the bottom left corner.
 * \param t Pointer to the triangle.
 * \param color The color of the triangle.
 * \param fill Boolean whether the shape is filled.
 * \param window Pointer to the target window.
 * \return true if draw succeeds.
 */
bool tri2d_t(const triangle_t *t, color_t color, bool fill, window_t *window) {
  if (t == NULL || window == NULL) return false;

  return tri2d((int)t->p0.x, (int)t->p0.y, (int)t->p1.x, (int)t->p1.y,
               (int)t->p2.x, (int)t->p2.y, color, fill, window);
}

/**
 * Draw a rectangle onto the window's buffer. By default, the window have origin
 * coordinate (0, 0) at the bottom left corner.
 * \param x0 The horizontal coordinate of the 1st vertex.
 * \param y0 The vertical coordinate of the 1st vertex.
 * \param x1 The horizontal coordinate of the 2nd vertex.
 * \param y1 The vertical coordinate of the 2nd vertex.
 * \param x2 The horizontal coordinate of the 3rd vertex.
 * \param y2 The vertical coordinate of the 3rd vertex.
 * \param x3 The horizontal coordinate of the 4th vertex.
 * \param y3 The vertical coordinate of the 4th vertex.
 * \param color The color of the triangle.
 * \param fill Boolean whether the shape is filled.
 * \param window Pointer to the target window.
 * \return true if draw succeeds.
 */
bool rec2d(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3,
           color_t color, bool fill, window_t *window) {
  if (window == NULL) return false;

  int x_min = max(min(x0, min(x1, min(x2, x3))), 0);
  int x_max = min(max(x0, max(x1, max(x2, x3))), window->width - 1);
  int y_min = max(min(y0, min(y1, min(y2, y3))), 0);
  int y_max = min(max(y0, max(y1, max(y2, y3))), window->height - 1);

  if (fill) {
    for (int y = y_min; y <= y_max; y++) {
      for (int x = x_min; x <= x_max; x++) {
        pixel2d(x, y, color, window);
      }
    }
    return true;
  } else {
    return line2d(x_min, y_min, x_min, y_max, color, window) &&
           line2d(x_min, y_min, x_max, y_min, color, window) &&
           line2d(x_max, y_min, x_max, y_max, color, window) &&
           line2d(x_max, y_max, x_min, y_max, color, window);
  }
}

/**
 * Draw a rectangle onto the window's buffer. By default, the window have origin
 * coordinate (0, 0) at the bottom left corner.
 * \param r Pointer to the triangle.
 * \param color The color of the triangle.
 * \param fill Boolean whether the shape is filled.
 * \param window Pointer to the target window.
 * \return true if draw succeeds.
 */
bool rec2d_r(const rectangle_t *r, color_t color, bool fill, window_t *window) {
  if (r == NULL || window == NULL) return false;

  return rec2d((int)r->p0.x, (int)r->p0.y, (int)r->p1.x, (int)r->p1.y,
               (int)r->p2.x, (int)r->p2.y, (int)r->p3.x, (int)r->p3.y, color,
               fill, window);
}

/**
 * Draw a rectangle onto the window's buffer. By default, the window have origin
 * coordinate (0, 0) at the bottom left corner. Notice that the horizontal
 * coordinate of the rectangle is from x to x + width - 1 (inclusively). The
 * vertical coordinate of the rectangle is from y to y - height + 1
 * (inclusively).
 * \param x X of top left corner.
 * \param y Y of top left corner.
 * \param width Width of the rectangle.
 * \param height Height of the rectangle.
 * \param color The color of the triangle.
 * \param fill Boolean whether the shape is filled.
 * \param window Pointer to the target window.
 * \return true if draw succeeds.
 */
bool rec2d_wh(int x, int y, int width, int height, color_t color, bool fill,
              window_t *window) {
  if (window == NULL) return false;

  int x_end = min(x + width - 1, window->width - 1);
  int y_end = max(y - height + 1, 0);
  x = max(x, 0);
  y = min(y, window->height - 1);

  if (x >= window->width || x_end <= 0 || y_end >= window->height || y <= 0) {
    return true;
  }

  if (fill) {
    for (int c = y_end; c <= y; c++) {
      for (int r = x; r <= x_end; r++) {
        pixel2d(r, c, color, window);
      }
    }
  } else {
    line2d(x, y, x, y_end - 1, color, window);
    line2d(x, y, x_end - 1, y, color, window);
    line2d(x_end, y, x_end, y_end, color, window);
    line2d(x, y_end, x_end, y_end, color, window);
  }
  return true;
}

/******************************************************************************/
/**
 * Draw pixel onto the window's buffer. By default, the window have origin
 * coordinate (0, 0) at the bottom left corner.
 * \param x The horizontal coordinate of the pixel.
 * \param y The vertical coordinate of the pixel.
 * \param z The depth coordinate of the pixel.
 * \param color The color of the pixel.
 * \param window Pointer to the target window.
 * \return true if draw succeeds.
 */
bool pixel3d(int x, int y, int z, color_t color, window_t *window) {
  if (window == NULL) return false;
  size_t idx = x + y * window->width;

  if (window->z_buffer[idx] <= z && x < window->width && x >= 0 &&
      y < window->height && y >= 0) {
    window->addr[idx] = color;
    window->z_buffer[idx] = z;
  }
  return true;
}

/**
 * Draw pixel onto the window's buffer. By default, the window have origin
 * coordinate (0, 0) at the bottom left corner.
 * \param p Pointer to the point struct.
 * \param color The color of the pixel.
 * \param window Pointer to the target window.
 * \return true if draw succeeds.
 */
bool pixel3d_p(point_t *p, color_t color, window_t *window) {
  if (p == NULL || window == NULL) return false;

  return pixel3d((int)p->x, (int)p->y, (int)p->z, color, window);
}

/**
 * Credit: Dmitry V. Sokolov
 * Link:
 * https://github.com/ssloy/tinyrenderer/wiki/Lesson-1:-Bresenham’s-Line-Drawing-Algorithm
 *
 * Draw line onto the window's buffer. By default, the window have origin
 * coordinate (0, 0) at the bottom left corner.
 * \param x0 The horizontal coordinate of the start point.
 * \param y0 The vertical coordinate of the start point.
 * \param z0 The depth coordinate of the start point.
 * \param x1 The horizontal coordinate of the end point.
 * \param y1 The vertical coordinate of the end point.
 * \param z1 The depth coordinate of the end.
 * \param color The color of the line.
 * \param window Pointer to the target window.
 * \return true if draw succeeds.
 */
bool line3d(int x0, int y0, int z0, int x1, int y1, int z1, color_t color,
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
    swap(&z0, &z1);
  }
  int dx = x1 - x0;
  int dy = y1 - y0;
  int derror2 = abs(dy) * 2;
  int error2 = 0;
  int y = y0;
  float z = z0;
  if (steep) {
    for (int x = x0; x <= x1; x++) {
      z = z_line_itpl((float)x, (float)x0, (float)x1, (float)z0, (float)z1);
      pixel3d(y, x, (int)z, color, window);
      error2 += derror2;
      if (error2 > dx) {
        y += (y1 > y0 ? 1 : -1);
        error2 -= dx * 2;
      }
    }
  } else {
    for (int x = x0; x <= x1; x++) {
      z = z_line_itpl((float)x, (float)x0, (float)x1, (float)z0, (float)z1);
      pixel3d(x, y, z, color, window);
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
 * coordinate (0, 0) at the bottom left corner.
 * \param l Pointer to the line object.
 * \param color The color of the line.
 * \param window Pointer to the target window.
 * \return true if draw succeeds.
 */
bool line3d_l(const line_t *l, color_t color, window_t *window) {
  if (l == NULL || window == NULL) return false;

  return line3d((int)l->p0.x, (int)l->p0.y, (int)l->p0.z, (int)l->p1.x,
                (int)l->p1.y, (int)l->p1.z, color, window);
}

/**
 * Draw a triangle onto the window's buffer. By default, the window have origin
 * coordinate (0, 0) at the bottom left corner.
 * \param x0 The horizontal coordinate of the 1st vertex.
 * \param y0 The vertical coordinate of the 1st vertex.
 * \param z0 The depth coordinate of the 1st vertex.
 * \param x1 The horizontal coordinate of the 2nd vertex.
 * \param y1 The vertical coordinate of the 2nd vertex.
 * \param z1 The depth coordinate of the 2nd vertex.
 * \param x2 The horizontal coordinate of the 3rd vertex.
 * \param y2 The vertical coordinate of the 3rd vertex.
 * \param z2 The depth coordinate of the 3rd vertex.
 * \param color The color of the triangle.
 * \param fill Boolean whether the shape is filled.
 * \param window Pointer to the target window.
 * \return true if draw succeeds.
 */
bool tri3d(int x0, int y0, int z0, int x1, int y1, int z1, int x2, int y2,
           int z2, color_t color, bool fill, window_t *window) {
  if (window == NULL) return false;

  int x_min = max(min(x0, min(x1, x2)), 0);
  int x_max = min(max(x0, max(x1, x2)), window->width - 1);
  int y_min = max(min(y0, min(y1, y2)), 0);
  int y_max = min(max(y0, max(y1, y2)), window->height - 1);
  float dx10 = (float)(x1 - x0);
  float dy10 = (float)(y1 - y0);
  float dx02 = (float)(x0 - x2);
  float dy02 = (float)(y0 - y2);
  float dx21 = (float)(x2 - x1);
  float dy21 = (float)(y2 - y1);

  if (fill) {
    for (int y = y_min; y <= y_max; y++) {
      for (int x = x_min; x <= x_max; x++) {
        float dx0 = (float)(x0 - x);
        float dx1 = (float)(x1 - x);
        float dx2 = (float)(x2 - x);

        float dy0 = (float)(y0 - y);
        float dy1 = (float)(y1 - y);
        float dy2 = (float)(y2 - y);

        float z0_det = det2f(dx2, dy2, dx21, dy21);
        float z1_det = det2f(dx0, dy0, dx02, dy02);
        float z2_det = det2f(dx1, dy1, dx10, dy10);
        
        if ((z0_det >= 0 && z1_det >= 0 && z2_det >= 0) ||
            (z0_det <= 0 && z1_det <= 0 && z2_det <= 0)) {
          float sum_det = z0_det + z1_det + z2_det;
          float z = (float)z0 * (z0_det / sum_det) +
                    (float)z1 * (z1_det / sum_det) +
                    (float)z2 * (z2_det / sum_det);
          pixel3d(x, y, z, color, window);
        }
      }
    }
    return true;
  } else {
    return line3d(x0, y0, z0, x1, y1, z1, color, window) &&
           line3d(x1, y1, z1, x2, y2, z2, color, window) &&
           line3d(x2, y2, z2, x0, y0, z0, color, window);
  }
}

/**
 * Draw a triangle onto the window's buffer. By default, the window have origin
 * coordinate (0, 0) at the botom left corner.
 * \param t Pointer to the triangle.
 * \param color The color of the triangle.
 * \param fill Boolean whether the shape is filled.
 * \param window Pointer to the target window.
 * \return true if draw succeeds.
 */
bool tri3d_t(const triangle_t *t, color_t color, bool fill, window_t *window) {
  if (t == NULL || window == NULL) return false;

  return tri3d((int)t->p0.x, (int)t->p0.y, (int)t->p0.z, (int)t->p1.x,
               (int)t->p1.y, (int)t->p1.z, (int)t->p2.x, (int)t->p2.y,
               (int)t->p2.z, color, fill, window);
}

/******************************************************************************/
/**
 * Render object in 3D using orthogonal projection.
 * \param obj Pointer to the object.
 * \param translate Boolean to whether we translate the object.
 * \param rotate Boolean to whether we rotate the object.
 * \param scale Boolean to whether we scale the object.
 * \param fill Boolean to whether we fill the triangles.
 * \param window Pointer to the window.
 * return true if draw succeeds.
 */
bool obj3d_o(object_t *obj, bool translate, bool rotate, bool scale, bool fill,
             window_t *window) {
  if (obj == NULL || window == NULL) return false;

  // Compose transformation matrix
  fmat4x4_t tmat = id_mat;
  fmat4x4_t rmat = id_mat;
  fmat4x4_t smat = id_mat;
  fmat4x4_t transform_mat = id_mat;
  if (translate) translate_mat4x4(&tmat, obj->dx, obj->dy, obj->dz);
  if (rotate) rotate_mat4x4(&rmat, obj->rot_angle, &(obj->rot_axis));
  if (scale) scale_mat4x4(&smat, obj->sx, obj->sy, obj->sz);
  transform_mat4x4(&transform_mat, &tmat, &rmat, &smat);

  // Draw each triangle to the buffer
  triangle_t *cur_tri = NULL;
  triangle_t temp_tri;
  for (int64_t i = 0; i < obj->nb_tri; i++) {
    cur_tri = obj->mesh + i;

    fmat4x4_mult_vec4(&transform_mat, &(cur_tri->p0), &(temp_tri.p0));
    fmat4x4_mult_vec4(&transform_mat, &(cur_tri->p1), &(temp_tri.p1));
    fmat4x4_mult_vec4(&transform_mat, &(cur_tri->p2), &(temp_tri.p2));

    tri3d_t(&temp_tri, (obj->mess_color)[i], fill, window);
  }
  return true;
}