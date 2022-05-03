#include "vec.h"

/******************************************************************************/
// Vec2 int
ivec2_t ivec2_add(ivec2_t v1, ivec2_t v2) {
  ivec2_t res = {.x = v1.x + v2.x, .y = v1.y + v2.y};
  return res;
}

ivec2_t ivec2_sub(ivec2_t v1, ivec2_t v2) {
  ivec2_t res = {.x = v1.x - v2.x, .y = v1.y - v2.y};
  return res;
}

ivec2_t ivec2_mult(ivec2_t v1, ivec2_t v2) {
  ivec2_t res = {.x = v1.x * v2.x, .y = v1.y * v2.y};
  return res;
}

ivec2_t ivec2_scale(ivec2_t v1, int scale) {
  ivec2_t res = {.x = v1.x * scale, .y = v1.y * scale};
  return res;
}

int ivec2_dot_prod(ivec2_t v1, ivec2_t v2) { return v1.x * v2.x + v1.y * v2.y; }

int ivec2_mag_sq(ivec2_t v1) { return v1.x * v1.x + v1.y * v1.y; }

bool ivec2_equal(ivec2_t v1, ivec2_t v2) {
  return v1.x == v2.x && v1.y == v2.y;
}

/******************************************************************************/
// Vec2 float
fvec2_t fvec2_add(fvec2_t v1, fvec2_t v2) {
  fvec2_t res = {.x = v1.x + v2.x, .y = v1.y + v2.y};
  return res;
}

fvec2_t fvec2_sub(fvec2_t v1, fvec2_t v2) {
  fvec2_t res = {.x = v1.x - v2.x, .y = v1.y - v2.y};
  return res;
}

fvec2_t fvec2_ew_mult(fvec2_t v1, fvec2_t v2) {
  fvec2_t res = {.x = v1.x * v2.x, .y = v1.y * v2.y};
  return res;
}

fvec2_t fvec2_scale(fvec2_t v1, float scale) {
  fvec2_t res = {.x = v1.x * scale, .y = v1.y * scale};
  return res;
}

float fvec2_dot_prod(fvec2_t v1, fvec2_t v2) {
  return v1.x * v2.x + v1.y * v2.y;
}

float fvec2_mag_sq(fvec2_t v1) { return v1.x * v1.x + v1.y * v1.y; }

bool fvec2_equal(fvec2_t v1, fvec2_t v2) {
  return (int)(v1.x * 1000) == (int)(v2.x * 1000) &&
         (int)(v1.y * 1000) == (int)(v2.y * 1000);
}

/******************************************************************************/
// Matrix 2x2 int
imat2x2_t imat2x2_add(imat2x2_t m1, imat2x2_t m2) {
  imat2x2_t res = {.c1 = {.x = m1.c1.x + m2.c1.x, .y = m1.c1.y + m2.c1.y},
                   .c2 = {.x = m1.c2.x + m2.c2.x, .y = m1.c2.y + m2.c2.y}};
  return res;
}

imat2x2_t imat2x2_sub(imat2x2_t m1, imat2x2_t m2) {
  imat2x2_t res = {.c1 = {.x = m1.c1.x - m2.c1.x, .y = m1.c1.y - m2.c1.y},
                   .c2 = {.x = m1.c2.x - m2.c2.x, .y = m1.c2.y - m2.c2.y}};
  return res;
}

imat2x2_t imat2x2_scale(imat2x2_t m1, int scale) {
  imat2x2_t res = {.c1 = {.x = m1.c1.x * scale, .y = m1.c1.y * scale},
                   .c2 = {.x = m1.c2.x * scale, .y = m1.c2.y * scale}};
  return res;
}

imat2x2_t imat2x2_matmul(imat2x2_t m1, imat2x2_t m2) {
  imat2x2_t res = {.c1 = {.x = m1.c1.x * m2.c1.x + m1.c2.x * m2.c1.y,
                          .y = m1.c1.y * m2.c1.x + m1.c2.y * m2.c1.y},
                   .c2 = {.x = m1.c1.x * m2.c2.x + m1.c2.x * m2.c2.y,
                          .y = m1.c1.y * m2.c2.x + m1.c2.y * m2.c2.y}};
  return res;
}

bool imat2x2_equal(imat2x2_t m1, imat2x2_t m2) {
  return ivec2_equal(m1.c1, m2.c1) && ivec2_equal(m1.c2, m2.c2);
}

/******************************************************************************/
// Matrix 2x2 float
fmat2x2_t fmat2x2_add(fmat2x2_t m1, fmat2x2_t m2) {
  fmat2x2_t res = {.c1 = {.x = m1.c1.x + m2.c1.x, .y = m1.c1.y + m2.c1.y},
                   .c2 = {.x = m1.c2.x + m2.c2.x, .y = m1.c2.y + m2.c2.y}};
  return res;
}

fmat2x2_t fmat2x2_sub(fmat2x2_t m1, fmat2x2_t m2) {
  fmat2x2_t res = {.c1 = {.x = m1.c1.x - m2.c1.x, .y = m1.c1.y - m2.c1.y},
                   .c2 = {.x = m1.c2.x - m2.c2.x, .y = m1.c2.y - m2.c2.y}};
  return res;
}

fmat2x2_t fmat2x2_scale(fmat2x2_t m1, int scale) {
  fmat2x2_t res = {.c1 = {.x = m1.c1.x * scale, .y = m1.c1.y * scale},
                   .c2 = {.x = m1.c2.x * scale, .y = m1.c2.y * scale}};
  return res;
}

fmat2x2_t fmat2x2_matmul(fmat2x2_t m1, fmat2x2_t m2) {
  fmat2x2_t res = {.c1 = {.x = m1.c1.x * m2.c1.x + m1.c2.x * m2.c1.y,
                          .y = m1.c1.y * m2.c1.x + m1.c2.y * m2.c1.y},
                   .c2 = {.x = m1.c1.x * m2.c2.x + m1.c2.x * m2.c2.y,
                          .y = m1.c1.y * m2.c2.x + m1.c2.y * m2.c2.y}};
  return res;
}

bool fmat2x2_equal(fmat2x2_t m1, fmat2x2_t m2) {
  return fvec2_equal(m1.c1, m2.c1) && fvec2_equal(m1.c2, m2.c2);
}

/******************************************************************************/
// Vec4 int

ivec4_t ivec4_add(ivec4_t v1, ivec4_t v2) {
  ivec4_t res = {
      .x = v1.x + v2.x, .y = v1.y + v2.y, .z = v1.z + v2.z, .w = v1.w + v2.w};

  return res;
}

ivec4_t ivec4_sub(ivec4_t v1, ivec4_t v2) {
  ivec4_t res = {
      .x = v1.x - v2.x, .y = v1.y - v2.y, .z = v1.z - v2.z, .w = v1.w - v2.w};

  return res;
}

ivec4_t ivec4_mult(ivec4_t v1, ivec4_t v2) {
  ivec4_t res = {
      .x = v1.x * v2.x, .y = v1.y * v2.y, .z = v1.z * v2.z, .w = v1.w * v2.w};

  return res;
}

ivec4_t ivec4_scale(ivec4_t v1, int scale) {
  ivec4_t res = {.x = v1.x * scale,
                 .y = v1.y * scale,
                 .z = v1.z * scale,
                 .w = v1.w * scale};

  return res;
}

int ivec4_dot_prod(ivec4_t v1, ivec4_t v2) {
  return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w);
}

int ivec4_mag_sq(ivec4_t v1) {
  return (v1.x * v1.x) + (v1.y * v1.y) + (v1.z * v1.z) + (v1.w * v1.w);
}

bool ivec4_equal(ivec4_t v1, ivec4_t v2) {
  return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.w == v2.w;
}

fvec4_t fvec4_add(fvec4_t v1, fvec4_t v2) {
  fvec4_t res = {
      .x = v1.x + v2.x, .y = v1.y + v2.y, .z = v1.z + v2.z, .w = v1.w + v2.w};

  return res;
}

fvec4_t fvec4_sub(fvec4_t v1, fvec4_t v2) {
  fvec4_t res = {
      .x = v1.x - v2.x, .y = v1.y - v2.y, .z = v1.z - v2.z, .w = v1.w - v2.w};

  return res;
}

fvec4_t fvec4_ew_mult(fvec4_t v1, fvec4_t v2) {
  fvec4_t res = {
      .x = v1.x * v2.x, .y = v1.y * v2.y, .z = v1.z * v2.z, .w = v1.w * v2.w};

  return res;
}

fvec4_t fvec4_scale(fvec4_t v1, float scale) {
  fvec4_t res = {.x = v1.x * scale,
                 .y = v1.y * scale,
                 .z = v1.z * scale,
                 .w = v1.w * scale};

  return res;
}

float fvec4_dot_prod(fvec4_t v1, fvec4_t v2) {
  return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w);
}

int fvec4_mag_sq(fvec4_t v1) {
  return (v1.x * v1.x) + (v1.y * v1.y) + (v1.z * v1.z) + (v1.w * v1.w);
}

bool fvec4_equal(fvec4_t v1, fvec4_t v2) {
  return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.w == v2.w;
}

imat4x4_t imat4x4_add(imat4x4_t m1, imat4x4_t m2) {
  imat4x4_t res = {.c1 = {.x = m1.c1.x + m2.c1.x,
                          .y = m1.c1.y + m2.c1.y,
                          .z = m1.c1.z + m2.c1.z,
                          .w = m1.c1.w + m2.c1.w},
                   .c2 = {.x = m1.c2.x + m2.c2.x,
                          .y = m1.c2.y + m2.c2.y,
                          .z = m1.c2.z + m2.c2.z,
                          .w = m1.c2.w + m2.c2.w},
                   .c3 = {.x = m1.c3.x + m2.c3.x,
                          .y = m1.c3.y + m2.c3.y,
                          .z = m1.c3.z + m2.c3.z,
                          .w = m1.c3.w + m2.c3.w},
                   .c4 = {.x = m1.c4.x + m2.c4.x,
                          .y = m1.c4.y + m2.c4.y,
                          .z = m1.c4.z + m2.c4.z,
                          .w = m1.c4.w + m2.c4.w}};
  return res;
}

imat4x4_t imat4x4_sub(imat4x4_t m1, imat4x4_t m2) {
  imat4x4_t res = {.c1 = {.x = m1.c1.x - m2.c1.x,
                          .y = m1.c1.y - m2.c1.y,
                          .z = m1.c1.z - m2.c1.z,
                          .w = m1.c1.w - m2.c1.w},
                   .c2 = {.x = m1.c2.x - m2.c2.x,
                          .y = m1.c2.y - m2.c2.y,
                          .z = m1.c2.z - m2.c2.z,
                          .w = m1.c2.w - m2.c2.w},
                   .c3 = {.x = m1.c3.x - m2.c3.x,
                          .y = m1.c3.y - m2.c3.y,
                          .z = m1.c3.z - m2.c3.z,
                          .w = m1.c3.w - m2.c3.w},
                   .c4 = {.x = m1.c4.x - m2.c4.x,
                          .y = m1.c4.y - m2.c4.y,
                          .z = m1.c4.z - m2.c4.z,
                          .w = m1.c4.w - m2.c4.w}};
  return res;
}

imat4x4_t imat4x4_scale(imat4x4_t m1, int scale) {
  imat4x4_t res = {.c1 = {.x = m1.c1.x * scale,
                          .y = m1.c1.y * scale,
                          .z = m1.c1.z * scale,
                          .w = m1.c1.w * scale},
                   .c2 = {.x = m1.c2.x * scale,
                          .y = m1.c2.y * scale,
                          .z = m1.c2.z * scale,
                          .w = m1.c2.w * scale},
                   .c3 = {.x = m1.c3.x * scale,
                          .y = m1.c3.y * scale,
                          .z = m1.c3.z * scale,
                          .w = m1.c3.w * scale},
                   .c4 = {.x = m1.c4.x * scale,
                          .y = m1.c4.y * scale,
                          .z = m1.c4.z * scale,
                          .w = m1.c4.w * scale}};
  return res;
}

imat4x4_t imat4x4_matmul(imat4x4_t m1, imat4x4_t m2) {
  imat4x4_t res = {.c1 = {.x = m1.c1.x * m2.c1.x + m1.c2.x * m2.c1.y +
                               m1.c3.x * m2.c1.z + m1.c4.x * m2.c1.w,
                          .y = m1.c1.y * m2.c1.x + m1.c2.y * m2.c1.y +
                               m1.c3.y * m2.c1.z + m1.c4.y * m2.c1.w,
                          .z = m1.c1.z * m2.c1.x + m1.c2.z * m2.c1.y +
                               m1.c3.z * m2.c1.z + m1.c4.z * m2.c1.w,
                          .w = m1.c1.w * m2.c1.x + m1.c2.w * m2.c1.y +
                               m1.c3.w * m2.c1.z + m1.c4.w * m2.c1.w},
                   .c2 = {.x = m1.c1.x * m2.c2.x + m1.c2.x * m2.c2.y +
                               m1.c3.x * m2.c2.z + m1.c4.x * m2.c2.w,
                          .y = m1.c1.y * m2.c2.x + m1.c2.y * m2.c2.y +
                               m1.c3.y * m2.c2.z + m1.c4.y * m2.c2.w,
                          .z = m1.c1.z * m2.c2.z + m1.c2.z * m2.c2.y +
                               m1.c3.z * m2.c2.z + m1.c4.z * m2.c2.w,
                          .w = m1.c1.w * m2.c2.x + m1.c2.w * m2.c2.y +
                               m1.c3.w * m2.c2.z + m1.c4.w * m2.c2.w},
                   .c3 = {.x = m1.c1.x * m2.c3.x + m1.c2.x * m2.c3.y +
                               m1.c3.x * m2.c3.z + m1.c4.x * m2.c3.w,
                          .y = m1.c1.y * m2.c3.x + m1.c2.y * m2.c3.y +
                               m1.c3.y * m2.c3.z + m1.c4.y * m2.c3.w,
                          .z = m1.c1.z * m2.c3.x + m1.c2.z * m2.c3.y +
                               m1.c3.z * m2.c3.z + m1.c4.z * m2.c3.w,
                          .w = m1.c1.w * m2.c3.x + m1.c2.w * m2.c3.w +
                               m1.c3.w * m2.c3.z + m1.c4.w * m2.c3.w},
                   .c4 = {.x = m1.c1.x * m2.c4.x + m1.c2.x * m2.c4.y +
                               m1.c3.x * m2.c4.z + m1.c4.x * m2.c4.w,
                          .y = m1.c1.y * m2.c4.x + m1.c2.y * m2.c4.y +
                               m1.c3.y * m2.c4.z + m1.c4.y * m2.c4.w,
                          .z = m1.c1.z * m2.c4.x + m1.c2.z * m2.c4.y +
                               m1.c3.z * m2.c4.z + m1.c4.z * m2.c4.w,
                          .w = m1.c1.w * m2.c4.x + m1.c2.w * m2.c4.y +
                               m1.c3.w * m2.c4.z + m1.c4.w * m2.c4.w}};

  return res;
}

bool imat4x4_equal(imat4x4_t m1, imat4x4_t m2) {
  return ivec4_equal(m1.c1, m2.c1) && ivec4_equal(m1.c2, m2.c2) &&
         ivec4_equal(m1.c3, m2.c3) && ivec4_equal(m1.c4, m2.c4);
}

fmat4x4_t fmat4x4_add(fmat4x4_t m1, fmat4x4_t m2) {
  fmat4x4_t res = {.c1 = {.x = m1.c1.x + m2.c1.x,
                          .y = m1.c1.y + m2.c1.y,
                          .z = m1.c1.z + m2.c1.z,
                          .w = m1.c1.w + m2.c1.w},
                   .c2 = {.x = m1.c2.x + m2.c2.x,
                          .y = m1.c2.y + m2.c2.y,
                          .z = m1.c2.z + m2.c2.z,
                          .w = m1.c2.w + m2.c2.w},
                   .c3 = {.x = m1.c3.x + m2.c3.x,
                          .y = m1.c3.y + m2.c3.y,
                          .z = m1.c3.z + m2.c3.z,
                          .w = m1.c3.w + m2.c3.w},
                   .c4 = {.x = m1.c4.x + m2.c4.x,
                          .y = m1.c4.y + m2.c4.y,
                          .z = m1.c4.z + m2.c4.z,
                          .w = m1.c4.w + m2.c4.w}};
  return res;
}

fmat4x4_t imat4x4_sub(fmat4x4_t m1, fmat4x4_t m2) {
  fmat4x4_t res = {.c1 = {.x = m1.c1.x - m2.c1.x,
                          .y = m1.c1.y - m2.c1.y,
                          .z = m1.c1.z - m2.c1.z,
                          .w = m1.c1.w - m2.c1.w},
                   .c2 = {.x = m1.c2.x - m2.c2.x,
                          .y = m1.c2.y - m2.c2.y,
                          .z = m1.c2.z - m2.c2.z,
                          .w = m1.c2.w - m2.c2.w},
                   .c3 = {.x = m1.c3.x - m2.c3.x,
                          .y = m1.c3.y - m2.c3.y,
                          .z = m1.c3.z - m2.c3.z,
                          .w = m1.c3.w - m2.c3.w},
                   .c4 = {.x = m1.c4.x - m2.c4.x,
                          .y = m1.c4.y - m2.c4.y,
                          .z = m1.c4.z - m2.c4.z,
                          .w = m1.c4.w - m2.c4.w}};
  return res;
}

fmat4x4_t imat4x4_scale(fmat4x4_t m1, int scale) {
  fmat4x4_t res = {.c1 = {.x = m1.c1.x * scale,
                          .y = m1.c1.y * scale,
                          .z = m1.c1.z * scale,
                          .w = m1.c1.w * scale},
                   .c2 = {.x = m1.c2.x * scale,
                          .y = m1.c2.y * scale,
                          .z = m1.c2.z * scale,
                          .w = m1.c2.w * scale},
                   .c3 = {.x = m1.c3.x * scale,
                          .y = m1.c3.y * scale,
                          .z = m1.c3.z * scale,
                          .w = m1.c3.w * scale},
                   .c4 = {.x = m1.c4.x * scale,
                          .y = m1.c4.y * scale,
                          .z = m1.c4.z * scale,
                          .w = m1.c4.w * scale}};
  return res;
}

fmat4x4_t fmat4x4_matmul(fmat4x4_t m1, fmat4x4_t m2) {
  fmat4x4_t res = {.c1 = {.x = m1.c1.x * m2.c1.x + m1.c2.x * m2.c1.y +
                               m1.c3.x * m2.c1.z + m1.c4.x * m2.c1.w,
                          .y = m1.c1.y * m2.c1.x + m1.c2.y * m2.c1.y +
                               m1.c3.y * m2.c1.z + m1.c4.y * m2.c1.w,
                          .z = m1.c1.z * m2.c1.x + m1.c2.z * m2.c1.y +
                               m1.c3.z * m2.c1.z + m1.c4.z * m2.c1.w,
                          .w = m1.c1.w * m2.c1.x + m1.c2.w * m2.c1.y +
                               m1.c3.w * m2.c1.z + m1.c4.w * m2.c1.w},
                   .c2 = {.x = m1.c1.x * m2.c2.x + m1.c2.x * m2.c2.y +
                               m1.c3.x * m2.c2.z + m1.c4.x * m2.c2.w,
                          .y = m1.c1.y * m2.c2.x + m1.c2.y * m2.c2.y +
                               m1.c3.y * m2.c2.z + m1.c4.y * m2.c2.w,
                          .z = m1.c1.z * m2.c2.z + m1.c2.z * m2.c2.y +
                               m1.c3.z * m2.c2.z + m1.c4.z * m2.c2.w,
                          .w = m1.c1.w * m2.c2.x + m1.c2.w * m2.c2.y +
                               m1.c3.w * m2.c2.z + m1.c4.w * m2.c2.w},
                   .c3 = {.x = m1.c1.x * m2.c3.x + m1.c2.x * m2.c3.y +
                               m1.c3.x * m2.c3.z + m1.c4.x * m2.c3.w,
                          .y = m1.c1.y * m2.c3.x + m1.c2.y * m2.c3.y +
                               m1.c3.y * m2.c3.z + m1.c4.y * m2.c3.w,
                          .z = m1.c1.z * m2.c3.x + m1.c2.z * m2.c3.y +
                               m1.c3.z * m2.c3.z + m1.c4.z * m2.c3.w,
                          .w = m1.c1.w * m2.c3.x + m1.c2.w * m2.c3.w +
                               m1.c3.w * m2.c3.z + m1.c4.w * m2.c3.w},
                   .c4 = {.x = m1.c1.x * m2.c4.x + m1.c2.x * m2.c4.y +
                               m1.c3.x * m2.c4.z + m1.c4.x * m2.c4.w,
                          .y = m1.c1.y * m2.c4.x + m1.c2.y * m2.c4.y +
                               m1.c3.y * m2.c4.z + m1.c4.y * m2.c4.w,
                          .z = m1.c1.z * m2.c4.x + m1.c2.z * m2.c4.y +
                               m1.c3.z * m2.c4.z + m1.c4.z * m2.c4.w,
                          .w = m1.c1.w * m2.c4.x + m1.c2.w * m2.c4.y +
                               m1.c3.w * m2.c4.z + m1.c4.w * m2.c4.w}};

  return res;
}

bool fmat4x4_equal(fmat4x4_t m1, fmat4x4_t m2) {
  return fvec4_equal(m1.c1, m2.c1) && fvec4_equal(m1.c2, m2.c2) &&
         fvec4_equal(m1.c3, m2.c3) && fvec4_equal(m1.c4, m2.c4);
}
