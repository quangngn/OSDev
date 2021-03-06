#include "graphic_transform.h"

/**
 * Construct a translation matrix based on given movement in x, y, and z
 * direction. The result is stored in transl_mat.
 * \returns true if succeed, false other wise.
 */
bool translate_mat4x4(fmat4x4_t* transl_mat, float dx, float dy, float dz) {
  if (transl_mat == NULL) return false;

  transl_mat->c1.x = 1;
  transl_mat->c1.y = 0;
  transl_mat->c1.z = 0;
  transl_mat->c1.w = 0;

  transl_mat->c2.x = 0;
  transl_mat->c2.y = 1;
  transl_mat->c2.z = 0;
  transl_mat->c2.w = 0;

  transl_mat->c3.x = 0;
  transl_mat->c3.y = 0;
  transl_mat->c3.z = 1;
  transl_mat->c3.w = 0;

  transl_mat->c4.x = dx;
  transl_mat->c4.y = dy;
  transl_mat->c4.z = dz;
  transl_mat->c4.w = 1;

  return true;
}

/**
 * Construct a scaling matrix based on given scale of in x, y, and z direction.
 * The result is stored in scale_mat.
 * \returns true if succeed, false otherwise.
 */
bool scale_mat4x4(fmat4x4_t* scale_mat, float sx, float sy, float sz) {
  if (scale_mat == NULL) return false;

  scale_mat->c1.x = sx;
  scale_mat->c1.y = 0;
  scale_mat->c1.z = 0;
  scale_mat->c1.w = 0;

  scale_mat->c2.x = 0;
  scale_mat->c2.y = sy;
  scale_mat->c2.z = 0;
  scale_mat->c2.w = 0;

  scale_mat->c3.x = 0;
  scale_mat->c3.y = 0;
  scale_mat->c3.z = sz;
  scale_mat->c3.w = 0;

  scale_mat->c4.x = 0;
  scale_mat->c4.y = 0;
  scale_mat->c4.z = 0;
  scale_mat->c4.w = 1;

  return true;
}

/**
 * Source: https://en.wikipedia.org/wiki/Rotation_matrix
 * 
 * Construct a rotation matrix based on given the angle and an axis. The axis
 * must be a unit vector. The result is stored in rot_mat.
 * \returns true if succeed, false otherwise.
 */
bool rotate_mat4x4(fmat4x4_t* rot_mat, float angle, fvec4_t* axis) {
  if (rot_mat == NULL || axis == NULL) return false;

  float x = axis->x;
  float y = axis->y;
  float z = axis->z;

  float cos_theta = cos(angle);
  float sin_theta = sin(angle);
  float m_cos_theta = 1 - cos_theta;

  rot_mat->c1.x = x * x * m_cos_theta + cos_theta;
  rot_mat->c1.y = x * y * m_cos_theta + z * sin_theta;
  rot_mat->c1.z = x * z * m_cos_theta - y * sin_theta;
  rot_mat->c4.w = 0;

  rot_mat->c2.x = y * x * m_cos_theta - z * sin_theta;
  rot_mat->c2.y = y * y * m_cos_theta + cos_theta;
  rot_mat->c2.z = y * z * m_cos_theta + x * sin_theta;
  rot_mat->c2.w = 0;

  rot_mat->c3.x = z * x * m_cos_theta + y * sin_theta;
  rot_mat->c3.y = z * y * m_cos_theta - x * sin_theta;
  rot_mat->c3.z = z * z * m_cos_theta + cos_theta;
  rot_mat->c4.w = 0;

  rot_mat->c4.x = 0;
  rot_mat->c4.y = 0;
  rot_mat->c4.z = 0;
  rot_mat->c4.w = 1;

  return true;
}

/**
 * Construct a general transformation matrix from the given translation,
 * rotation, and scale matrix. The order is as follows:
 * transf = transl x rotation x scale
 * \returns true if succeed, false otherwise.
 */
bool transform_mat4x4(fmat4x4_t* transform_mat, fmat4x4_t* translate_mat,
                      fmat4x4_t* rot_mat, fmat4x4_t* scale_mat) {
  if (transform_mat == NULL || translate_mat == NULL || rot_mat == NULL ||
      scale_mat == NULL) {
    return false;
  }

  return fmat4x4_matmul(translate_mat, rot_mat, transform_mat) &&
         fmat4x4_matmul(transform_mat, scale_mat, transform_mat);
}