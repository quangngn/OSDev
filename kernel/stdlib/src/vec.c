#include "vec.h"

/******************************************************************************/
// Vec2 int
bool ivec2_add(ivec2_t *v1, ivec2_t *v2, ivec2_t *res) {
  if (v1 == NULL || v2 == NULL || res == NULL) return false;

  res->x = v1->x + v2->x;
  res->y = v1->y + v2->y;

  return true;
}

bool ivec2_sub(ivec2_t *v1, ivec2_t *v2, ivec2_t *res) {
  if (v1 == NULL || v2 == NULL || res == NULL) return false;

  res->x = v1->x - v2->x;
  res->y = v1->y - v2->y;

  return true;
}

bool ivec2_mult(ivec2_t *v1, ivec2_t *v2, ivec2_t *res) {
  if (v1 == NULL || v2 == NULL || res == NULL) return false;

  res->x = v1->x * v2->x;
  res->y = v1->y * v2->y;

  return true;
}

bool ivec2_scale(ivec2_t *v1, int scale, ivec2_t *res) {
  if (v1 == NULL || res == NULL) return false;

  res->x = v1->x * scale;
  res->y = v1->y * scale;

  return true;
}

bool ivec2_dot_prod(ivec2_t *v1, ivec2_t *v2, int *res) {
  if (v1 == NULL || v2 == NULL || res == NULL) return false;

  *res = v1->x * v2->x + v1->y * v2->y;

  return true;
}

bool ivec2_mag_sq(ivec2_t *v1, int *res) {
  if (v1 == NULL || res == NULL) return false;

  *res = v1->x * v1->x + v1->y * v1->y;

  return true;
}

bool ivec2_equal(ivec2_t *v1, ivec2_t *v2) {
  if (v1 == NULL || v2 == NULL) return false;

  return v1->x == v2->x && v1->y == v2->y;
}

/******************************************************************************/
// Vec2 float
bool fvec2_add(fvec2_t *v1, fvec2_t *v2, fvec2_t *res) {
  if (v1 == NULL || v2 == NULL || res == NULL) return false;

  res->x = v1->x + v2->x;
  res->y = v1->y + v2->y;

  return true;
}

bool fvec2_sub(fvec2_t *v1, fvec2_t *v2, fvec2_t *res) {
  if (v1 == NULL || v2 == NULL || res == NULL) return false;

  res->x = v1->x - v2->x;
  res->y = v1->y - v2->y;

  return true;
}

bool fvec2_mult(fvec2_t *v1, fvec2_t *v2, fvec2_t *res) {
  if (v1 == NULL || v2 == NULL || res == NULL) return false;

  res->x = v1->x * v2->x;
  res->y = v1->y * v2->y;

  return true;
}

bool fvec2_scale(fvec2_t *v1, float scale, fvec2_t *res) {
  if (v1 == NULL || res == NULL) return false;

  res->x = v1->x * scale;
  res->y = v1->y * scale;

  return true;
}

bool fvec2_dot_prod(fvec2_t *v1, fvec2_t *v2, float *res) {
  if (v1 == NULL || v2 == NULL || res == NULL) return false;

  *res = v1->x * v2->x + v1->y * v2->y;

  return true;
}

bool fvec2_mag_sq(fvec2_t *v1, float *res) {
  if (v1 == NULL || res == NULL) return false;

  *res = v1->x * v1->x + v1->y * v1->y;

  return true;
}

bool fvec2_equal(fvec2_t *v1, fvec2_t *v2) {
  if (v1 == NULL || v2 == NULL) return false;

  return (int)(v1->x * 1000) == (int)(v2->x * 1000) &&
         (int)(v1->y * 1000) == (int)(v2->y * 1000);
}

/******************************************************************************/
// Matrix 2x2 int
bool imat2x2_add(imat2x2_t *m1, imat2x2_t *m2, imat2x2_t *res) {
  if (m1 == NULL || m2 == NULL || res == NULL) return false;

  res->c1.x = m1->c1.x + m2->c1.x;
  res->c1.y = m1->c1.y + m2->c1.y;
  res->c2.x = m1->c2.x + m2->c2.x;
  res->c2.y = m1->c2.y + m2->c2.y;

  return true;
}

bool imat2x2_sub(imat2x2_t *m1, imat2x2_t *m2, imat2x2_t *res) {
  if (m1 == NULL || m2 == NULL || res == NULL) return false;

  res->c1.x = m1->c1.x - m2->c1.x;
  res->c1.y = m1->c1.y - m2->c1.y;
  res->c2.x = m1->c2.x - m2->c2.x;
  res->c2.y = m1->c2.y - m2->c2.y;

  return true;
}

bool imat2x2_scale(imat2x2_t *m1, int scale, imat2x2_t *res) {
  if (m1 == NULL || res == NULL) return false;

  res->c1.x = m1->c1.x * scale;
  res->c1.y = m1->c1.y * scale;
  res->c2.x = m1->c2.x * scale;
  res->c2.y = m1->c2.y * scale;

  return true;
}

bool imat2x2_matmul(imat2x2_t *m1, imat2x2_t *m2, imat2x2_t *res) {
  if (m1 == NULL || res == NULL) return false;

  res->c1.x = m1->c1.x * m2->c1.x + m1->c2.x * m2->c1.y;
  res->c1.y = m1->c1.y * m2->c1.x + m1->c2.y * m2->c1.y;
  res->c2.x = m1->c1.x * m2->c2.x + m1->c2.x * m2->c2.y;
  res->c2.y = m1->c1.y * m2->c2.x + m1->c2.y * m2->c2.y;

  return true;
}

bool imat2x2_equal(imat2x2_t *m1, imat2x2_t *m2) {
  if (m1 == NULL || m2 == NULL) return false;

  return ivec2_equal(&(m1->c1), &(m2->c1)) && ivec2_equal(&(m1->c2), &(m2->c2));
}

/******************************************************************************/
// Matrix 2x2 float
bool fmat2x2_add(fmat2x2_t *m1, fmat2x2_t *m2, fmat2x2_t *res) {
  if (m1 == NULL || m2 == NULL || res == NULL) return false;

  res->c1.x = m1->c1.x + m2->c1.x;
  res->c1.y = m1->c1.y + m2->c1.y;
  res->c2.x = m1->c2.x + m2->c2.x;
  res->c2.y = m1->c2.y + m2->c2.y;

  return true;
}

bool fmat2x2_sub(fmat2x2_t *m1, fmat2x2_t *m2, fmat2x2_t *res) {
  if (m1 == NULL || m2 == NULL || res == NULL) return false;

  res->c1.x = m1->c1.x - m2->c1.x;
  res->c1.y = m1->c1.y - m2->c1.y;
  res->c2.x = m1->c2.x - m2->c2.x;
  res->c2.y = m1->c2.y - m2->c2.y;

  return true;
}

bool fmat2x2_scale(fmat2x2_t *m1, float scale, fmat2x2_t *res) {
  if (m1 == NULL || res == NULL) return false;

  res->c1.x = m1->c1.x * scale;
  res->c1.y = m1->c1.y * scale;
  res->c2.x = m1->c2.x * scale;
  res->c2.y = m1->c2.y * scale;

  return true;
}

bool fmat2x2_matmul(fmat2x2_t *m1, fmat2x2_t *m2, fmat2x2_t *res) {
  if (m1 == NULL || res == NULL) return false;

  res->c1.x = m1->c1.x * m2->c1.x + m1->c2.x * m2->c1.y;
  res->c1.y = m1->c1.y * m2->c1.x + m1->c2.y * m2->c1.y;
  res->c2.x = m1->c1.x * m2->c2.x + m1->c2.x * m2->c2.y;
  res->c2.y = m1->c1.y * m2->c2.x + m1->c2.y * m2->c2.y;

  return true;
}

bool fmat2x2_equal(fmat2x2_t *m1, fmat2x2_t *m2) {
  if (m1 == NULL || m2 == NULL) return false;

  return fvec2_equal(&(m1->c1), &(m2->c1)) && fvec2_equal(&(m1->c2), &(m2->c2));
}

/******************************************************************************/
// Vec4 int
bool ivec4_add(ivec4_t *v1, ivec4_t *v2, ivec4_t *res) {
  if (v1 == NULL || v2 == NULL || res == NULL) return false;

  res->x = v1->x + v2->x;
  res->y = v1->y + v2->y;
  res->z = v1->z + v2->z;
  res->w = v1->w + v2->w;

  return true;
}

bool ivec4_sub(ivec4_t *v1, ivec4_t *v2, ivec4_t *res) {
  if (v1 == NULL || v2 == NULL || res == NULL) return false;

  res->x = v1->x - v2->x;
  res->y = v1->y - v2->y;
  res->z = v1->z - v2->z;
  res->w = v1->w - v2->w;

  return true;
}

bool ivec4_mult(ivec4_t *v1, ivec4_t *v2, ivec4_t *res) {
  if (v1 == NULL || v2 == NULL || res == NULL) return false;

  res->x = v1->x * v2->x;
  res->y = v1->y * v2->y;
  res->z = v1->z * v2->z;
  res->w = v1->w * v2->w;

  return true;
}

bool ivec4_scale(ivec4_t *v1, int scale, ivec4_t *res) {
  if (v1 == NULL || res == NULL) return false;

  res->x = v1->x * scale;
  res->y = v1->y * scale;
  res->z = v1->z * scale;
  res->w = v1->w * scale;

  return true;
}

int ivec4_dot_prod(ivec4_t *v1, ivec4_t *v2, int *res) {
  if (v1 == NULL || v2 == NULL || res == NULL) return false;

  *res = v1->x * v2->x + v1->y * v2->y + v1->z * v2->z + v1->w * v2->w;

  return true;
}

int ivec4_mag_sq(ivec4_t *v1, int *res) {
  if (v1 == NULL || res == NULL) return false;

  *res = v1->x * v1->x + v1->y * v1->y + v1->z * v1->z + v1->w * v1->w;

  return true;
}

bool ivec4_equal(ivec4_t *v1, ivec4_t *v2) {
  if (v1 == NULL || v2 == NULL) return false;

  return v1->x == v2->x && v1->y == v2->y && v1->z == v2->z && v1->w == v2->w;
}

/******************************************************************************/
// Vec4 float
bool fvec4_add(fvec4_t *v1, fvec4_t *v2, fvec4_t *res) {
  if (v1 == NULL || v2 == NULL || res == NULL) return false;

  res->x = v1->x + v2->x;
  res->y = v1->y + v2->y;
  res->z = v1->z + v2->z;
  res->w = v1->w + v2->w;

  return true;
}

bool fvec4_sub(fvec4_t *v1, fvec4_t *v2, fvec4_t *res) {
  if (v1 == NULL || v2 == NULL || res == NULL) return false;

  res->x = v1->x - v2->x;
  res->y = v1->y - v2->y;
  res->z = v1->z - v2->z;
  res->w = v1->w - v2->w;

  return true;
}

bool fvec4_mult(fvec4_t *v1, fvec4_t *v2, fvec4_t *res) {
  if (v1 == NULL || v2 == NULL || res == NULL) return false;

  res->x = v1->x * v2->x;
  res->y = v1->y * v2->y;
  res->z = v1->z * v2->z;
  res->w = v1->w * v2->w;

  return true;
}

bool fvec4_scale(fvec4_t *v1, float scale, fvec4_t *res) {
  if (v1 == NULL || res == NULL) return false;

  res->x = v1->x * scale;
  res->y = v1->y * scale;
  res->z = v1->z * scale;
  res->w = v1->w * scale;

  return true;
}

float fvec4_dot_prod(fvec4_t *v1, fvec4_t *v2, float *res) {
  if (v1 == NULL || v2 == NULL || res == NULL) return false;

  *res = v1->x * v2->x + v1->y * v2->y + v1->z * v2->z + v1->w * v2->w;

  return true;
}

float fvec4_mag_sq(fvec4_t *v1, float *res) {
  if (v1 == NULL || res == NULL) return false;

  *res = v1->x * v1->x + v1->y * v1->y + v1->z * v1->z + v1->w * v1->w;

  return true;
}

bool fvec4_equal(fvec4_t *v1, fvec4_t *v2) {
  if (v1 == NULL || v2 == NULL) return false;

  return (int)(v1->x * 1000) == (int)(v2->x * 1000) &&
         (int)(v1->y * 1000) == (int)(v2->y * 1000) &&
         (int)(v1->z * 1000) == (int)(v2->z * 1000) &&
         (int)(v1->w * 1000) == (int)(v2->w * 1000);
}

/******************************************************************************/
// Mat4x4 int
bool imat4x4_add(imat4x4_t *m1, imat4x4_t *m2, imat4x4_t *res) {
  if (m1 == NULL || m2 == NULL || res == NULL) return false;

  res->c1.x = m1->c1.x + m2->c1.x;
  res->c1.y = m1->c1.y + m2->c1.y;
  res->c1.z = m1->c1.z + m2->c1.z;
  res->c1.w = m1->c1.w + m2->c1.w;

  res->c2.x = m1->c2.x + m2->c2.x;
  res->c2.y = m1->c2.y + m2->c2.y;
  res->c2.z = m1->c2.z + m2->c2.z;
  res->c2.w = m1->c2.w + m2->c2.w;

  res->c3.x = m1->c3.x + m2->c3.x;
  res->c3.y = m1->c3.y + m2->c3.y;
  res->c3.z = m1->c3.z + m2->c3.z;
  res->c3.w = m1->c3.w + m2->c3.w;

  res->c4.x = m1->c4.x + m2->c4.x;
  res->c4.y = m1->c4.y + m2->c4.y;
  res->c4.z = m1->c4.z + m2->c4.z;
  res->c4.w = m1->c4.w + m2->c4.w;

  return true;
}

bool imat4x4_sub(imat4x4_t *m1, imat4x4_t *m2, imat4x4_t *res) {
  if (m1 == NULL || m2 == NULL || res == NULL) return false;

  res->c1.x = m1->c1.x - m2->c1.x;
  res->c1.y = m1->c1.y - m2->c1.y;
  res->c1.z = m1->c1.z - m2->c1.z;
  res->c1.w = m1->c1.w - m2->c1.w;

  res->c2.x = m1->c2.x - m2->c2.x;
  res->c2.y = m1->c2.y - m2->c2.y;
  res->c2.z = m1->c2.z - m2->c2.z;
  res->c2.w = m1->c2.w - m2->c2.w;

  res->c3.x = m1->c3.x - m2->c3.x;
  res->c3.y = m1->c3.y - m2->c3.y;
  res->c3.z = m1->c3.z - m2->c3.z;
  res->c3.w = m1->c3.w - m2->c3.w;

  res->c4.x = m1->c4.x - m2->c4.x;
  res->c4.y = m1->c4.y - m2->c4.y;
  res->c4.z = m1->c4.z - m2->c4.z;
  res->c4.w = m1->c4.w - m2->c4.w;

  return true;
}

bool imat4x4_matmul(imat4x4_t *m1, imat4x4_t *m2, imat4x4_t *res) {
  if (m1 == NULL || m2 == NULL || res == NULL) return false;

  res->c1.x = m1->c1.x * m2->c1.x + m1->c2.x * m2->c1.y + m1->c3.x * m2->c1.z +
              m1->c4.x * m2->c1.w;
  res->c1.y = m1->c1.y * m2->c1.x + m1->c2.y * m2->c1.y + m1->c3.y * m2->c1.z +
              m1->c4.y * m2->c1.w;
  res->c1.z = m1->c1.z * m2->c1.x + m1->c2.z * m2->c1.y + m1->c3.z * m2->c1.z +
              m1->c4.z * m2->c1.w;
  res->c1.w = m1->c1.w * m2->c1.x + m1->c2.w * m2->c1.y + m1->c3.w * m2->c1.z +
              m1->c4.w * m2->c1.w;

  res->c2.x = m1->c1.x * m2->c2.x + m1->c2.x * m2->c2.y + m1->c3.x * m2->c2.z +
              m1->c4.x * m2->c2.w;
  res->c2.y = m1->c1.y * m2->c2.x + m1->c2.y * m2->c2.y + m1->c3.y * m2->c2.z +
              m1->c4.y * m2->c2.w;
  res->c2.z = m1->c1.z * m2->c2.x + m1->c2.z * m2->c2.y + m1->c3.z * m2->c2.z +
              m1->c4.z * m2->c2.w;
  res->c2.w = m1->c1.w * m2->c2.x + m1->c2.w * m2->c2.y + m1->c3.w * m2->c2.z +
              m1->c4.w * m2->c2.w;

  res->c3.x = m1->c1.x * m2->c3.x + m1->c2.x * m2->c3.y + m1->c3.x * m2->c3.z +
              m1->c4.x * m2->c3.w;
  res->c3.y = m1->c1.y * m2->c3.x + m1->c2.y * m2->c3.y + m1->c3.y * m2->c3.z +
              m1->c4.y * m2->c3.w;
  res->c3.z = m1->c1.z * m2->c3.x + m1->c2.z * m2->c3.y + m1->c3.z * m2->c3.z +
              m1->c4.z * m2->c3.w;
  res->c3.w = m1->c1.w * m2->c3.x + m1->c2.w * m2->c3.y + m1->c3.w * m2->c3.z +
              m1->c4.w * m2->c3.w;

  res->c4.x = m1->c1.x * m2->c4.x + m1->c2.x * m2->c4.y + m1->c3.x * m2->c4.z +
              m1->c4.x * m2->c4.w;
  res->c4.y = m1->c1.y * m2->c4.x + m1->c2.y * m2->c4.y + m1->c3.y * m2->c4.z +
              m1->c4.y * m2->c4.w;
  res->c4.z = m1->c1.z * m2->c4.x + m1->c2.z * m2->c4.y + m1->c3.z * m2->c4.z +
              m1->c4.z * m2->c3.w;
  res->c4.w = m1->c1.w * m2->c4.x + m1->c2.w * m2->c4.y + m1->c3.w * m2->c4.z +
              m1->c4.w * m2->c4.w;

  return true;
}

bool imat4x4_scale(imat4x4_t *m1, int scale, imat4x4_t *res) {
  if (m1 == NULL || res == NULL) return false;

  res->c1.x = m1->c1.x * scale;
  res->c1.y = m1->c1.y * scale;
  res->c1.z = m1->c1.z * scale;
  res->c1.w = m1->c1.w * scale;

  res->c2.x = m1->c2.x * scale;
  res->c2.y = m1->c2.y * scale;
  res->c2.z = m1->c2.z * scale;
  res->c2.w = m1->c2.w * scale;

  res->c3.x = m1->c3.x * scale;
  res->c3.y = m1->c3.y * scale;
  res->c3.z = m1->c3.z * scale;
  res->c3.w = m1->c3.w * scale;

  res->c4.x = m1->c4.x * scale;
  res->c4.y = m1->c4.y * scale;
  res->c4.z = m1->c4.z * scale;
  res->c4.w = m1->c4.w * scale;

  return true;
}

bool imat4x4_equal(imat4x4_t *m1, imat4x4_t *m2) {
  if (m1 == NULL || m2 == NULL) return false;

  return ivec4_equal(&(m1->c1), &(m2->c1)) &&
         ivec4_equal(&(m1->c2), &(m2->c2)) &&
         ivec4_equal(&(m1->c3), &(m2->c3)) && ivec4_equal(&(m1->c4), &(m2->c4));
}

/******************************************************************************/
// Mat4x4 float
bool fmat4x4_add(fmat4x4_t *m1, fmat4x4_t *m2, fmat4x4_t *res) {
  if (m1 == NULL || m2 == NULL || res == NULL) return false;

  res->c1.x = m1->c1.x + m2->c1.x;
  res->c1.y = m1->c1.y + m2->c1.y;
  res->c1.z = m1->c1.z + m2->c1.z;
  res->c1.w = m1->c1.w + m2->c1.w;

  res->c2.x = m1->c2.x + m2->c2.x;
  res->c2.y = m1->c2.y + m2->c2.y;
  res->c2.z = m1->c2.z + m2->c2.z;
  res->c2.w = m1->c2.w + m2->c2.w;

  res->c3.x = m1->c3.x + m2->c3.x;
  res->c3.y = m1->c3.y + m2->c3.y;
  res->c3.z = m1->c3.z + m2->c3.z;
  res->c3.w = m1->c3.w + m2->c3.w;

  res->c4.x = m1->c4.x + m2->c4.x;
  res->c4.y = m1->c4.y + m2->c4.y;
  res->c4.z = m1->c4.z + m2->c4.z;
  res->c4.w = m1->c4.w + m2->c4.w;

  return true;
}

bool fmat4x4_sub(fmat4x4_t *m1, fmat4x4_t *m2, fmat4x4_t *res) {
  if (m1 == NULL || m2 == NULL || res == NULL) return false;

  res->c1.x = m1->c1.x - m2->c1.x;
  res->c1.y = m1->c1.y - m2->c1.y;
  res->c1.z = m1->c1.z - m2->c1.z;
  res->c1.w = m1->c1.w - m2->c1.w;

  res->c2.x = m1->c2.x - m2->c2.x;
  res->c2.y = m1->c2.y - m2->c2.y;
  res->c2.z = m1->c2.z - m2->c2.z;
  res->c2.w = m1->c2.w - m2->c2.w;

  res->c3.x = m1->c3.x - m2->c3.x;
  res->c3.y = m1->c3.y - m2->c3.y;
  res->c3.z = m1->c3.z - m2->c3.z;
  res->c3.w = m1->c3.w - m2->c3.w;

  res->c4.x = m1->c4.x - m2->c4.x;
  res->c4.y = m1->c4.y - m2->c4.y;
  res->c4.z = m1->c4.z - m2->c4.z;
  res->c4.w = m1->c4.w - m2->c4.w;

  return true;
}

bool fmat4x4_matmul(fmat4x4_t *m1, fmat4x4_t *m2, fmat4x4_t *res) {
  if (m1 == NULL || m2 == NULL || res == NULL) return false;

  res->c1.x = m1->c1.x * m2->c1.x + m1->c2.x * m2->c1.y + m1->c3.x * m2->c1.z +
              m1->c4.x * m2->c1.w;
  res->c1.y = m1->c1.y * m2->c1.x + m1->c2.y * m2->c1.y + m1->c3.y * m2->c1.z +
              m1->c4.y * m2->c1.w;
  res->c1.z = m1->c1.z * m2->c1.x + m1->c2.z * m2->c1.y + m1->c3.z * m2->c1.z +
              m1->c4.z * m2->c1.w;
  res->c1.w = m1->c1.w * m2->c1.x + m1->c2.w * m2->c1.y + m1->c3.w * m2->c1.z +
              m1->c4.w * m2->c1.w;

  res->c2.x = m1->c1.x * m2->c2.x + m1->c2.x * m2->c2.y + m1->c3.x * m2->c2.z +
              m1->c4.x * m2->c2.w;
  res->c2.y = m1->c1.y * m2->c2.x + m1->c2.y * m2->c2.y + m1->c3.y * m2->c2.z +
              m1->c4.y * m2->c2.w;
  res->c2.z = m1->c1.z * m2->c2.x + m1->c2.z * m2->c2.y + m1->c3.z * m2->c2.z +
              m1->c4.z * m2->c2.w;
  res->c2.w = m1->c1.w * m2->c2.x + m1->c2.w * m2->c2.y + m1->c3.w * m2->c2.z +
              m1->c4.w * m2->c2.w;

  res->c3.x = m1->c1.x * m2->c3.x + m1->c2.x * m2->c3.y + m1->c3.x * m2->c3.z +
              m1->c4.x * m2->c3.w;
  res->c3.y = m1->c1.y * m2->c3.x + m1->c2.y * m2->c3.y + m1->c3.y * m2->c3.z +
              m1->c4.y * m2->c3.w;
  res->c3.z = m1->c1.z * m2->c3.x + m1->c2.z * m2->c3.y + m1->c3.z * m2->c3.z +
              m1->c4.z * m2->c3.w;
  res->c3.w = m1->c1.w * m2->c3.x + m1->c2.w * m2->c3.y + m1->c3.w * m2->c3.z +
              m1->c4.w * m2->c3.w;

  res->c4.x = m1->c1.x * m2->c4.x + m1->c2.x * m2->c4.y + m1->c3.x * m2->c4.z +
              m1->c4.x * m2->c4.w;
  res->c4.y = m1->c1.y * m2->c4.x + m1->c2.y * m2->c4.y + m1->c3.y * m2->c4.z +
              m1->c4.y * m2->c4.w;
  res->c4.z = m1->c1.z * m2->c4.x + m1->c2.z * m2->c4.y + m1->c3.z * m2->c4.z +
              m1->c4.z * m2->c3.w;
  res->c4.w = m1->c1.w * m2->c4.x + m1->c2.w * m2->c4.y + m1->c3.w * m2->c4.z +
              m1->c4.w * m2->c4.w;

  return true;
}

bool fmat4x4_scale(fmat4x4_t *m1, float scale, fmat4x4_t *res) {
  if (m1 == NULL || res == NULL) return false;

  res->c1.x = m1->c1.x * scale;
  res->c1.y = m1->c1.y * scale;
  res->c1.z = m1->c1.z * scale;
  res->c1.w = m1->c1.w * scale;

  res->c2.x = m1->c2.x * scale;
  res->c2.y = m1->c2.y * scale;
  res->c2.z = m1->c2.z * scale;
  res->c2.w = m1->c2.w * scale;

  res->c3.x = m1->c3.x * scale;
  res->c3.y = m1->c3.y * scale;
  res->c3.z = m1->c3.z * scale;
  res->c3.w = m1->c3.w * scale;

  res->c4.x = m1->c4.x * scale;
  res->c4.y = m1->c4.y * scale;
  res->c4.z = m1->c4.z * scale;
  res->c4.w = m1->c4.w * scale;

  return true;
}

bool fmat4x4_equal(fmat4x4_t *m1, fmat4x4_t *m2) {
  if (m1 == NULL || m2 == NULL) return false;

  return fvec4_equal(&(m1->c1), &(m2->c1)) &&
         fvec4_equal(&(m1->c2), &(m2->c2)) &&
         fvec4_equal(&(m1->c3), &(m2->c3)) && fvec4_equal(&(m1->c4), &(m2->c4));
}

bool fmat4x1_matmul(fmat4x4_t *m1, fmat4x1_t *m2, fmat4x1_t *res) {
  if (m1 == NULL || m2 == NULL || res == NULL) return false;

  res->c1.x = m1->c1.x * m2->c1.x + m1->c2.x * m2->c1.y + m1->c3.x * m2->c1.z +
              m1->c4.x * m2->c1.w;
  res->c1.y = m1->c1.y * m2->c1.x + m1->c2.y * m2->c1.y + m1->c3.y * m2->c1.z +
              m1->c4.y * m2->c1.w;
  res->c1.z = m1->c1.z * m2->c1.x + m1->c2.z * m2->c1.y + m1->c3.z * m2->c1.z +
              m1->c4.z * m2->c1.w;
  res->c1.w = m1->c1.w * m2->c1.x + m1->c2.w * m2->c1.y + m1->c3.w * m2->c1.z +
              m1->c4.w * m2->c1.w;

  return true;
}