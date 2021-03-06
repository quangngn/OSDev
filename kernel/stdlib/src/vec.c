#include "vec.h"

/******************************************************************************/
// Vec2 int
bool ivec2_add(ivec2_t *v1, ivec2_t *v2, ivec2_t *res) {
  if (v1 == NULL || v2 == NULL || res == NULL) return false;

  ivec2_t temp;
  temp.x = v1->x + v2->x;
  temp.y = v1->y + v2->y;
  memcpy(res, &temp, sizeof(ivec2_t));

  return true;
}

bool ivec2_sub(ivec2_t *v1, ivec2_t *v2, ivec2_t *res) {
  if (v1 == NULL || v2 == NULL || res == NULL) return false;

  ivec2_t temp;
  temp.x = v1->x - v2->x;
  temp.y = v1->y - v2->y;
  memcpy(res, &temp, sizeof(ivec2_t));

  return true;
}

bool ivec2_mult(ivec2_t *v1, ivec2_t *v2, ivec2_t *res) {
  if (v1 == NULL || v2 == NULL || res == NULL) return false;

  ivec2_t temp;
  temp.x = v1->x * v2->x;
  temp.y = v1->y * v2->y;
  memcpy(res, &temp, sizeof(ivec2_t));

  return true;
}

bool ivec2_scale(ivec2_t *v1, int scale, ivec2_t *res) {
  if (v1 == NULL || res == NULL) return false;

  ivec2_t temp;
  temp.x = v1->x * scale;
  temp.y = v1->y * scale;
  memcpy(res, &temp, sizeof(ivec2_t));

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

  fvec2_t temp;
  temp.x = v1->x + v2->x;
  temp.y = v1->y + v2->y;
  memcpy(res, &temp, sizeof(fvec2_t));

  return true;
}

bool fvec2_sub(fvec2_t *v1, fvec2_t *v2, fvec2_t *res) {
  if (v1 == NULL || v2 == NULL || res == NULL) return false;

  fvec2_t temp;
  temp.x = v1->x - v2->x;
  temp.y = v1->y - v2->y;
  memcpy(res, &temp, sizeof(fvec2_t));

  return true;
}

bool fvec2_mult(fvec2_t *v1, fvec2_t *v2, fvec2_t *res) {
  if (v1 == NULL || v2 == NULL || res == NULL) return false;

  fvec2_t temp;
  temp.x = v1->x * v2->x;
  temp.y = v1->y * v2->y;
  memcpy(res, &temp, sizeof(fvec2_t));

  return true;
}

bool fvec2_scale(fvec2_t *v1, float scale, fvec2_t *res) {
  if (v1 == NULL || res == NULL) return false;

  fvec2_t temp;
  temp.x = v1->x * scale;
  temp.y = v1->y * scale;
  memcpy(res, &temp, sizeof(fvec2_t));

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

  imat2x2_t temp;
  temp.c1.x = m1->c1.x + m2->c1.x;
  temp.c1.y = m1->c1.y + m2->c1.y;
  temp.c2.x = m1->c2.x + m2->c2.x;
  temp.c2.y = m1->c2.y + m2->c2.y;
  memcpy(res, &temp, sizeof(imat2x2_t));

  return true;
}

bool imat2x2_sub(imat2x2_t *m1, imat2x2_t *m2, imat2x2_t *res) {
  if (m1 == NULL || m2 == NULL || res == NULL) return false;

  imat2x2_t temp;
  temp.c1.x = m1->c1.x - m2->c1.x;
  temp.c1.y = m1->c1.y - m2->c1.y;
  temp.c2.x = m1->c2.x - m2->c2.x;
  temp.c2.y = m1->c2.y - m2->c2.y;
  memcpy(res, &temp, sizeof(imat2x2_t));

  return true;
}

bool imat2x2_scale(imat2x2_t *m1, int scale, imat2x2_t *res) {
  if (m1 == NULL || res == NULL) return false;

  imat2x2_t temp;
  temp.c1.x = m1->c1.x * scale;
  temp.c1.y = m1->c1.y * scale;
  temp.c2.x = m1->c2.x * scale;
  temp.c2.y = m1->c2.y * scale;
  memcpy(res, &temp, sizeof(imat2x2_t));

  return true;
}

bool imat2x2_matmul(imat2x2_t *m1, imat2x2_t *m2, imat2x2_t *res) {
  if (m1 == NULL || res == NULL) return false;

  imat2x2_t temp;
  temp.c1.x = m1->c1.x * m2->c1.x + m1->c2.x * m2->c1.y;
  temp.c1.y = m1->c1.y * m2->c1.x + m1->c2.y * m2->c1.y;
  temp.c2.x = m1->c1.x * m2->c2.x + m1->c2.x * m2->c2.y;
  temp.c2.y = m1->c1.y * m2->c2.x + m1->c2.y * m2->c2.y;
  memcpy(res, &temp, sizeof(imat2x2_t));

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

  fmat2x2_t temp;
  temp.c1.x = m1->c1.x + m2->c1.x;
  temp.c1.y = m1->c1.y + m2->c1.y;
  temp.c2.x = m1->c2.x + m2->c2.x;
  temp.c2.y = m1->c2.y + m2->c2.y;
  memcpy(res, &temp, sizeof(fmat2x2_t));

  return true;
}

bool fmat2x2_sub(fmat2x2_t *m1, fmat2x2_t *m2, fmat2x2_t *res) {
  if (m1 == NULL || m2 == NULL || res == NULL) return false;

  fmat2x2_t temp;
  temp.c1.x = m1->c1.x - m2->c1.x;
  temp.c1.y = m1->c1.y - m2->c1.y;
  temp.c2.x = m1->c2.x - m2->c2.x;
  temp.c2.y = m1->c2.y - m2->c2.y;
  memcpy(res, &temp, sizeof(fmat2x2_t));

  return true;
}

bool fmat2x2_scale(fmat2x2_t *m1, float scale, fmat2x2_t *res) {
  if (m1 == NULL || res == NULL) return false;

  fmat2x2_t temp;
  temp.c1.x = m1->c1.x * scale;
  temp.c1.y = m1->c1.y * scale;
  temp.c2.x = m1->c2.x * scale;
  temp.c2.y = m1->c2.y * scale;
  memcpy(res, &temp, sizeof(fmat2x2_t));

  return true;
}

bool fmat2x2_matmul(fmat2x2_t *m1, fmat2x2_t *m2, fmat2x2_t *res) {
  if (m1 == NULL || res == NULL) return false;

  fmat2x2_t temp;
  temp.c1.x = m1->c1.x * m2->c1.x + m1->c2.x * m2->c1.y;
  temp.c1.y = m1->c1.y * m2->c1.x + m1->c2.y * m2->c1.y;
  temp.c2.x = m1->c1.x * m2->c2.x + m1->c2.x * m2->c2.y;
  temp.c2.y = m1->c1.y * m2->c2.x + m1->c2.y * m2->c2.y;
  memcpy(res, &temp, sizeof(fmat2x2_t));

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

  ivec4_t temp;
  temp.x = v1->x + v2->x;
  temp.y = v1->y + v2->y;
  temp.z = v1->z + v2->z;
  temp.w = v1->w + v2->w;
  memcpy(res, &temp, sizeof(ivec4_t));

  return true;
}

bool ivec4_sub(ivec4_t *v1, ivec4_t *v2, ivec4_t *res) {
  if (v1 == NULL || v2 == NULL || res == NULL) return false;

  ivec4_t temp;
  temp.x = v1->x - v2->x;
  temp.y = v1->y - v2->y;
  temp.z = v1->z - v2->z;
  temp.w = v1->w - v2->w;
  memcpy(res, &temp, sizeof(ivec4_t));

  return true;
}

bool ivec4_mult(ivec4_t *v1, ivec4_t *v2, ivec4_t *res) {
  if (v1 == NULL || v2 == NULL || res == NULL) return false;

  ivec4_t temp;
  temp.x = v1->x * v2->x;
  temp.y = v1->y * v2->y;
  temp.z = v1->z * v2->z;
  temp.w = v1->w * v2->w;
  memcpy(res, &temp, sizeof(ivec4_t));

  return true;
}

bool ivec4_scale(ivec4_t *v1, int scale, ivec4_t *res) {
  if (v1 == NULL || res == NULL) return false;

  ivec4_t temp;
  temp.x = v1->x * scale;
  temp.y = v1->y * scale;
  temp.z = v1->z * scale;
  temp.w = v1->w * scale;
  memcpy(res, &temp, sizeof(ivec4_t));

  return true;
}

bool ivec4_dot_prod(ivec4_t *v1, ivec4_t *v2, int *res) {
  if (v1 == NULL || v2 == NULL || res == NULL) return false;

  *res = v1->x * v2->x + v1->y * v2->y + v1->z * v2->z + v1->w * v2->w;

  return true;
}

bool ivec4_mag_sq(ivec4_t *v1, int *res) {
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

  fvec4_t temp;
  temp.x = v1->x + v2->x;
  temp.y = v1->y + v2->y;
  temp.z = v1->z + v2->z;
  temp.w = v1->w + v2->w;
  memcpy(res, &temp, sizeof(fvec4_t));

  return true;
}

bool fvec4_sub(fvec4_t *v1, fvec4_t *v2, fvec4_t *res) {
  if (v1 == NULL || v2 == NULL || res == NULL) return false;

   fvec4_t temp;
  temp.x = v1->x - v2->x;
  temp.y = v1->y - v2->y;
  temp.z = v1->z - v2->z;
  temp.w = v1->w - v2->w;
  memcpy(res, &temp, sizeof(fvec4_t));

  return true;
}

bool fvec4_mult(fvec4_t *v1, fvec4_t *v2, fvec4_t *res) {
  if (v1 == NULL || v2 == NULL || res == NULL) return false;

   fvec4_t temp;
  temp.x = v1->x * v2->x;
  temp.y = v1->y * v2->y;
  temp.z = v1->z * v2->z;
  temp.w = v1->w * v2->w;
  memcpy(res, &temp, sizeof(fvec4_t));

  return true;
}

bool fvec4_scale(fvec4_t *v1, float scale, fvec4_t *res) {
  if (v1 == NULL || res == NULL) return false;

  fvec4_t temp;
  res->x = v1->x * scale;
  res->y = v1->y * scale;
  res->z = v1->z * scale;
  res->w = v1->w * scale;
  memcpy(res, &temp, sizeof(fvec4_t));

  return true;
}

bool fvec4_dot_prod(fvec4_t *v1, fvec4_t *v2, float *res) {
  if (v1 == NULL || v2 == NULL || res == NULL) return false;

  *res = v1->x * v2->x + v1->y * v2->y + v1->z * v2->z + v1->w * v2->w;

  return true;
}

bool fvec4_mag_sq(fvec4_t *v1, float *res) {
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

  imat4x4_t temp;
  temp.c1.x = m1->c1.x + m2->c1.x;
  temp.c1.y = m1->c1.y + m2->c1.y;
  temp.c1.z = m1->c1.z + m2->c1.z;
  temp.c1.w = m1->c1.w + m2->c1.w;

  temp.c2.x = m1->c2.x + m2->c2.x;
  temp.c2.y = m1->c2.y + m2->c2.y;
  temp.c2.z = m1->c2.z + m2->c2.z;
  temp.c2.w = m1->c2.w + m2->c2.w;

  temp.c3.x = m1->c3.x + m2->c3.x;
  temp.c3.y = m1->c3.y + m2->c3.y;
  temp.c3.z = m1->c3.z + m2->c3.z;
  temp.c3.w = m1->c3.w + m2->c3.w;

  temp.c4.x = m1->c4.x + m2->c4.x;
  temp.c4.y = m1->c4.y + m2->c4.y;
  temp.c4.z = m1->c4.z + m2->c4.z;
  temp.c4.w = m1->c4.w + m2->c4.w;
  memcpy(res, &temp, sizeof(imat4x4_t));

  return true;
}

bool imat4x4_sub(imat4x4_t *m1, imat4x4_t *m2, imat4x4_t *res) {
  if (m1 == NULL || m2 == NULL || res == NULL) return false;

  imat4x4_t temp;
  temp.c1.x = m1->c1.x - m2->c1.x;
  temp.c1.y = m1->c1.y - m2->c1.y;
  temp.c1.z = m1->c1.z - m2->c1.z;
  temp.c1.w = m1->c1.w - m2->c1.w;

  temp.c2.x = m1->c2.x - m2->c2.x;
  temp.c2.y = m1->c2.y - m2->c2.y;
  temp.c2.z = m1->c2.z - m2->c2.z;
  temp.c2.w = m1->c2.w - m2->c2.w;

  temp.c3.x = m1->c3.x - m2->c3.x;
  temp.c3.y = m1->c3.y - m2->c3.y;
  temp.c3.z = m1->c3.z - m2->c3.z;
  temp.c3.w = m1->c3.w - m2->c3.w;

  temp.c4.x = m1->c4.x - m2->c4.x;
  temp.c4.y = m1->c4.y - m2->c4.y;
  temp.c4.z = m1->c4.z - m2->c4.z;
  temp.c4.w = m1->c4.w - m2->c4.w;
  memcpy(res, &temp, sizeof(imat4x4_t));

  return true;
}

bool imat4x4_matmul(imat4x4_t *m1, imat4x4_t *m2, imat4x4_t *res) {
  if (m1 == NULL || m2 == NULL || res == NULL) return false;

  imat4x4_t temp;
  temp.c1.x = m1->c1.x * m2->c1.x + m1->c2.x * m2->c1.y + m1->c3.x * m2->c1.z +
              m1->c4.x * m2->c1.w;
  temp.c1.y = m1->c1.y * m2->c1.x + m1->c2.y * m2->c1.y + m1->c3.y * m2->c1.z +
              m1->c4.y * m2->c1.w;
  temp.c1.z = m1->c1.z * m2->c1.x + m1->c2.z * m2->c1.y + m1->c3.z * m2->c1.z +
              m1->c4.z * m2->c1.w;
  temp.c1.w = m1->c1.w * m2->c1.x + m1->c2.w * m2->c1.y + m1->c3.w * m2->c1.z +
              m1->c4.w * m2->c1.w;

  temp.c2.x = m1->c1.x * m2->c2.x + m1->c2.x * m2->c2.y + m1->c3.x * m2->c2.z +
              m1->c4.x * m2->c2.w;
  temp.c2.y = m1->c1.y * m2->c2.x + m1->c2.y * m2->c2.y + m1->c3.y * m2->c2.z +
              m1->c4.y * m2->c2.w;
  temp.c2.z = m1->c1.z * m2->c2.x + m1->c2.z * m2->c2.y + m1->c3.z * m2->c2.z +
              m1->c4.z * m2->c2.w;
  temp.c2.w = m1->c1.w * m2->c2.x + m1->c2.w * m2->c2.y + m1->c3.w * m2->c2.z +
              m1->c4.w * m2->c2.w;

  temp.c3.x = m1->c1.x * m2->c3.x + m1->c2.x * m2->c3.y + m1->c3.x * m2->c3.z +
              m1->c4.x * m2->c3.w;
  temp.c3.y = m1->c1.y * m2->c3.x + m1->c2.y * m2->c3.y + m1->c3.y * m2->c3.z +
              m1->c4.y * m2->c3.w;
  temp.c3.z = m1->c1.z * m2->c3.x + m1->c2.z * m2->c3.y + m1->c3.z * m2->c3.z +
              m1->c4.z * m2->c3.w;
  temp.c3.w = m1->c1.w * m2->c3.x + m1->c2.w * m2->c3.y + m1->c3.w * m2->c3.z +
              m1->c4.w * m2->c3.w;

  temp.c4.x = m1->c1.x * m2->c4.x + m1->c2.x * m2->c4.y + m1->c3.x * m2->c4.z +
              m1->c4.x * m2->c4.w;
  temp.c4.y = m1->c1.y * m2->c4.x + m1->c2.y * m2->c4.y + m1->c3.y * m2->c4.z +
              m1->c4.y * m2->c4.w;
  temp.c4.z = m1->c1.z * m2->c4.x + m1->c2.z * m2->c4.y + m1->c3.z * m2->c4.z +
              m1->c4.z * m2->c3.w;
  temp.c4.w = m1->c1.w * m2->c4.x + m1->c2.w * m2->c4.y + m1->c3.w * m2->c4.z +
              m1->c4.w * m2->c4.w;
  memcpy(res, &temp, sizeof(imat4x4_t));

  return true;
}

bool imat4x4_scale(imat4x4_t *m1, int scale, imat4x4_t *res) {
  if (m1 == NULL || res == NULL) return false;

  imat4x4_t temp;
  temp.c1.x = m1->c1.x * scale;
  temp.c1.y = m1->c1.y * scale;
  temp.c1.z = m1->c1.z * scale;
  temp.c1.w = m1->c1.w * scale;

  temp.c2.x = m1->c2.x * scale;
  temp.c2.y = m1->c2.y * scale;
  temp.c2.z = m1->c2.z * scale;
  temp.c2.w = m1->c2.w * scale;

  temp.c3.x = m1->c3.x * scale;
  temp.c3.y = m1->c3.y * scale;
  temp.c3.z = m1->c3.z * scale;
  temp.c3.w = m1->c3.w * scale;

  temp.c4.x = m1->c4.x * scale;
  temp.c4.y = m1->c4.y * scale;
  temp.c4.z = m1->c4.z * scale;
  temp.c4.w = m1->c4.w * scale;
  memcpy(res, &temp, sizeof(imat4x4_t));

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

  fmat4x4_t temp;
  temp.c1.x = m1->c1.x + m2->c1.x;
  temp.c1.y = m1->c1.y + m2->c1.y;
  temp.c1.z = m1->c1.z + m2->c1.z;
  temp.c1.w = m1->c1.w + m2->c1.w;

  temp.c2.x = m1->c2.x + m2->c2.x;
  temp.c2.y = m1->c2.y + m2->c2.y;
  temp.c2.z = m1->c2.z + m2->c2.z;
  temp.c2.w = m1->c2.w + m2->c2.w;

  temp.c3.x = m1->c3.x + m2->c3.x;
  temp.c3.y = m1->c3.y + m2->c3.y;
  temp.c3.z = m1->c3.z + m2->c3.z;
  temp.c3.w = m1->c3.w + m2->c3.w;

  temp.c4.x = m1->c4.x + m2->c4.x;
  temp.c4.y = m1->c4.y + m2->c4.y;
  temp.c4.z = m1->c4.z + m2->c4.z;
  temp.c4.w = m1->c4.w + m2->c4.w;
  memcpy(res, &temp, sizeof(fmat4x4_t));

  return true;
}

bool fmat4x4_sub(fmat4x4_t *m1, fmat4x4_t *m2, fmat4x4_t *res) {
  if (m1 == NULL || m2 == NULL || res == NULL) return false;

  fmat4x4_t temp;
  temp.c1.x = m1->c1.x - m2->c1.x;
  temp.c1.y = m1->c1.y - m2->c1.y;
  temp.c1.z = m1->c1.z - m2->c1.z;
  temp.c1.w = m1->c1.w - m2->c1.w;

  temp.c2.x = m1->c2.x - m2->c2.x;
  temp.c2.y = m1->c2.y - m2->c2.y;
  temp.c2.z = m1->c2.z - m2->c2.z;
  temp.c2.w = m1->c2.w - m2->c2.w;

  temp.c3.x = m1->c3.x - m2->c3.x;
  temp.c3.y = m1->c3.y - m2->c3.y;
  temp.c3.z = m1->c3.z - m2->c3.z;
  temp.c3.w = m1->c3.w - m2->c3.w;

  temp.c4.x = m1->c4.x - m2->c4.x;
  temp.c4.y = m1->c4.y - m2->c4.y;
  temp.c4.z = m1->c4.z - m2->c4.z;
  temp.c4.w = m1->c4.w - m2->c4.w;
  memcpy(res, &temp, sizeof(fmat4x4_t));

  return true;
}

bool fmat4x4_matmul(fmat4x4_t *m1, fmat4x4_t *m2, fmat4x4_t *res) {
  if (m1 == NULL || m2 == NULL || res == NULL) return false;

  fmat4x4_t temp;
  temp.c1.x = m1->c1.x * m2->c1.x + m1->c2.x * m2->c1.y + m1->c3.x * m2->c1.z +
              m1->c4.x * m2->c1.w;
  temp.c1.y = m1->c1.y * m2->c1.x + m1->c2.y * m2->c1.y + m1->c3.y * m2->c1.z +
              m1->c4.y * m2->c1.w;
  temp.c1.z = m1->c1.z * m2->c1.x + m1->c2.z * m2->c1.y + m1->c3.z * m2->c1.z +
              m1->c4.z * m2->c1.w;
  temp.c1.w = m1->c1.w * m2->c1.x + m1->c2.w * m2->c1.y + m1->c3.w * m2->c1.z +
              m1->c4.w * m2->c1.w;

  temp.c2.x = m1->c1.x * m2->c2.x + m1->c2.x * m2->c2.y + m1->c3.x * m2->c2.z +
              m1->c4.x * m2->c2.w;
  temp.c2.y = m1->c1.y * m2->c2.x + m1->c2.y * m2->c2.y + m1->c3.y * m2->c2.z +
              m1->c4.y * m2->c2.w;
  temp.c2.z = m1->c1.z * m2->c2.x + m1->c2.z * m2->c2.y + m1->c3.z * m2->c2.z +
              m1->c4.z * m2->c2.w;
  temp.c2.w = m1->c1.w * m2->c2.x + m1->c2.w * m2->c2.y + m1->c3.w * m2->c2.z +
              m1->c4.w * m2->c2.w;

  temp.c3.x = m1->c1.x * m2->c3.x + m1->c2.x * m2->c3.y + m1->c3.x * m2->c3.z +
              m1->c4.x * m2->c3.w;
  temp.c3.y = m1->c1.y * m2->c3.x + m1->c2.y * m2->c3.y + m1->c3.y * m2->c3.z +
              m1->c4.y * m2->c3.w;
  temp.c3.z = m1->c1.z * m2->c3.x + m1->c2.z * m2->c3.y + m1->c3.z * m2->c3.z +
              m1->c4.z * m2->c3.w;
  temp.c3.w = m1->c1.w * m2->c3.x + m1->c2.w * m2->c3.y + m1->c3.w * m2->c3.z +
              m1->c4.w * m2->c3.w;

  temp.c4.x = m1->c1.x * m2->c4.x + m1->c2.x * m2->c4.y + m1->c3.x * m2->c4.z +
              m1->c4.x * m2->c4.w;
  temp.c4.y = m1->c1.y * m2->c4.x + m1->c2.y * m2->c4.y + m1->c3.y * m2->c4.z +
              m1->c4.y * m2->c4.w;
  temp.c4.z = m1->c1.z * m2->c4.x + m1->c2.z * m2->c4.y + m1->c3.z * m2->c4.z +
              m1->c4.z * m2->c3.w;
  temp.c4.w = m1->c1.w * m2->c4.x + m1->c2.w * m2->c4.y + m1->c3.w * m2->c4.z +
              m1->c4.w * m2->c4.w;
  memcpy(res, &temp, sizeof(fmat4x4_t));

  return true;
}

bool fmat4x4_scale(fmat4x4_t *m1, float scale, fmat4x4_t *res) {
  if (m1 == NULL || res == NULL) return false;

  fmat4x4_t temp;
  temp.c1.x = m1->c1.x * scale;
  temp.c1.y = m1->c1.y * scale;
  temp.c1.z = m1->c1.z * scale;
  temp.c1.w = m1->c1.w * scale;

  temp.c2.x = m1->c2.x * scale;
  temp.c2.y = m1->c2.y * scale;
  temp.c2.z = m1->c2.z * scale;
  temp.c2.w = m1->c2.w * scale;

  temp.c3.x = m1->c3.x * scale;
  temp.c3.y = m1->c3.y * scale;
  temp.c3.z = m1->c3.z * scale;
  temp.c3.w = m1->c3.w * scale;

  temp.c4.x = m1->c4.x * scale;
  temp.c4.y = m1->c4.y * scale;
  temp.c4.z = m1->c4.z * scale;
  temp.c4.w = m1->c4.w * scale;
  memcpy(res, &temp, sizeof(fmat4x4_t));

  return true;
}

bool fmat4x4_equal(fmat4x4_t *m1, fmat4x4_t *m2) {
  if (m1 == NULL || m2 == NULL) return false;

  return fvec4_equal(&(m1->c1), &(m2->c1)) &&
         fvec4_equal(&(m1->c2), &(m2->c2)) &&
         fvec4_equal(&(m1->c3), &(m2->c3)) && fvec4_equal(&(m1->c4), &(m2->c4));
}

bool fmat4x4_mult_vec4(fmat4x4_t *m, fvec4_t *v, fvec4_t *res) {
  if (m == NULL || v == NULL || res == NULL) return false;

  fvec4_t temp;
  temp.x = m->c1.x * v->x + m->c2.x * v->y + m->c3.x * v->z + m->c4.x * v->w;
  temp.y = m->c1.y * v->x + m->c2.y * v->y + m->c3.y * v->z + m->c4.y * v->w;
  temp.z = m->c1.z * v->x + m->c2.z * v->y + m->c3.z * v->z + m->c4.z * v->w;
  temp.w = m->c1.w * v->x + m->c2.w * v->y + m->c3.w * v->z + m->c4.w * v->w;
  memcpy(res, &temp, sizeof(fvec4_t));
  
  return true;
}