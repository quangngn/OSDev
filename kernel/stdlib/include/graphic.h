#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "graphic_transform.h"
#include "math.h"
#include "mem.h"
#include "time.h"
// Color is defined in system.h

typedef struct {
  pixel_t* addr;
  int32_t* z_buffer;
  color_t bg;
  int32_t width;
  int32_t height;
  int32_t screen_x;
  int32_t screen_y;
  bool flip;
} window_t;

typedef struct {
  int32_t framebuffer_width;  // Width and height in pixels
  int32_t framebuffer_height;
  int32_t framebuffer_pitch;  // Pitch in bytes
  int32_t framebuffer_bpp;    // Bits per pixel
  uint8_t memory_model;       // Memory model: 1=RGB, all other values undefined
  uint8_t red_mask_size;      // RGB mask sizes and left shifts
  uint8_t red_mask_shift;
  uint8_t green_mask_size;
  uint8_t green_mask_shift;
  uint8_t blue_mask_size;
  uint8_t blue_mask_shift;
  uint8_t unused;
} framebuffer_info_t;

typedef fvec4_t point_t;

typedef struct {
  point_t p0;
  point_t p1;
} line_t;

typedef struct {
  point_t p0;
  point_t p1;
  point_t p2;
} triangle_t;

typedef struct {
  point_t p0;
  point_t p1;
  point_t p2;
  point_t p3;
} rectangle_t;

typedef struct {
  triangle_t* mesh;
  color_t* mess_color;
  int64_t nb_tri;
  fvec4_t rot_axis;
  float rot_angle;
  float sx;
  float sy;
  float sz;
  float dx;
  float dy;
  float dz;
} object_t;

/******************************************************************************/
// Graphical functions
/**
 * System call to get kernel's framebuffer's information. This information is
 * provided in the framebuffer struct tag.
 */
bool graphic_get_framebuffer_info(framebuffer_info_t* fb_info);

/**
 * Copy the current window's frame buffer to the kernel's framebuffer.
 * \param window Pointer to the window struct.
 * \param clear Clear window buffer after draw.
 * \return true if draw successfully.
 */
void graphic_draw(window_t* window_t, bool clear);

/**
 * Clear the kernel's buffer
 */ 
void graphic_clear_screen();

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
bool window_init(window_t* window, int width, int height, int screen_x,
                 int screen_y, color_t bg);

/**
 * Clear the window's framebuffer by setting to the default color.
 * \param window Pointer to the window struct.
 */
void window_clear(window_t* window);

/******************************************************************************/
// Draw primitive to user's framebuffer in 2D
/**
 * Draw pixel onto the window's buffer. By default, the window have origin
 * coordinate (0, 0) at the bottom left corner.
 * \param x The horizontal coordinate of the pixel.
 * \param y The vertical coordinate of the pixel.
 * \param color The color of the pixel.
 * \param window Pointer to the target window.
 * \return true if draw succeeds.
 */
bool pixel2d(int x, int y, color_t color, window_t* window);

/**
 * Draw pixel onto the window's buffer. By default, the window have origin
 * coordinate (0, 0) at the bottom left corner.
 * \param p Pointer to the point struct.
 * \param color The color of the pixel.
 * \param window Pointer to the target window.
 * \return true if draw succeeds.
 */
bool pixel2d_p(point_t* p, color_t color, window_t* window);

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
bool line2d(int x0, int y0, int x1, int y1, color_t color, window_t* window);

/**
 * Draw line onto the window's buffer. By default, the window have origin
 * coordinate (0, 0) at the bottom left corner.
 * \param l Pointer to the line object.
 * \param color The color of the line.
 * \param window Pointer to the target window.
 * \return true if draw succeeds.
 */
bool line2d_l(const line_t* l, color_t color, window_t* window);

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
           bool fill, window_t* window);

/**
 * Draw a triangle onto the window's buffer. By default, the window have origin
 * coordinate (0, 0) at the botom left corner.
 * \param t Pointer to the triangle.
 * \param color The color of the triangle.
 * \param fill Boolean whether the shape is filled.
 * \param window Pointer to the target window.
 * \return true if draw succeeds.
 */
bool tri2d_t(const triangle_t* t, color_t color, bool fill, window_t* window);
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
           color_t color, bool fill, window_t* window);

/**
 * Draw a rectangle onto the window's buffer. By default, the window have origin
 * coordinate (0, 0) at the bottom left corner.
 * \param r Pointer to the rectangle.
 * \param color The color of the triangle.
 * \param fill Boolean whether the shape is filled.
 * \param window Pointer to the target window.
 * \return true if draw succeeds.
 */
bool rec2d_r(const rectangle_t* r, color_t color, bool fill, window_t* window);

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
              window_t* window);

/******************************************************************************/
// Draw primitive to user's framebuffer in 3D
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
bool pixel3d(int x, int y, int z, color_t color, window_t* window);

/**
 * Draw pixel onto the window's buffer. By default, the window have origin
 * coordinate (0, 0) at the bottom left corner.
 * \param p Pointer to the point struct.
 * \param color The color of the pixel.
 * \param window Pointer to the target window.
 * \return true if draw succeeds.
 */
bool pixel3d_p(point_t* p, color_t color, window_t* window);

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
            window_t* window);

/**
 * Draw line onto the window's buffer. By default, the window have origin
 * coordinate (0, 0) at the bottom left corner.
 * \param l Pointer to the line object.
 * \param color The color of the line.
 * \param window Pointer to the target window.
 * \return true if draw succeeds.
 */
bool line3d_l(const line_t* l, color_t color, window_t* window);

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
           int z2, color_t color, bool fill, window_t* window);

/**
 * Draw a triangle onto the window's buffer. By default, the window have origin
 * coordinate (0, 0) at the botom left corner.
 * \param t Pointer to the triangle.
 * \param color The color of the triangle.
 * \param fill Boolean whether the shape is filled.
 * \param window Pointer to the target window.
 * \return true if draw succeeds.
 */
bool tri3d_t(const triangle_t* t, color_t color, bool fill, window_t* window);

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
bool obj3d_o(object_t* obj, bool translate, bool rotate, bool scale,
             bool fill, window_t* window);