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
