#include "graphic_transform.h"

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

bool rotate_mat4x4_x(fmat4x4_t* rot_mat, float theta) {
  if (rot_mat == NULL) return false;

  // Precompute trig values
  float sin_theta = sin(theta);
  float cos_theta = cos(theta);

  // Rotation
  rot_mat->c1.x = 1;
  rot_mat->c1.y = 0;
  rot_mat->c1.z = 0;
  rot_mat->c1.w = 0;

  rot_mat->c2.x = 0;
  rot_mat->c2.y = cos_theta;
  rot_mat->c2.z = sin_theta;
  rot_mat->c2.w = 0;

  rot_mat->c3.x = 0;
  rot_mat->c3.y = -sin_theta;
  rot_mat->c3.z = cos_theta;
  rot_mat->c3.w = 0;

  rot_mat->c4.x = 0;
  rot_mat->c4.y = 0;
  rot_mat->c4.z = 0;
  rot_mat->c4.w = 1;

  return true;
}

bool rotate_mat4x4_y(fmat4x4_t* rot_mat, float theta) {
  if (rot_mat == NULL) return false;

  // Precompute trig values
  float sin_theta = sin(theta);
  float cos_theta = cos(theta);

  // Rotation
  rot_mat->c1.x = cos_theta;
  rot_mat->c1.y = 0;
  rot_mat->c1.z = sin_theta;
  rot_mat->c1.w = 0;

  rot_mat->c2.x = 0;
  rot_mat->c2.y = 1;
  rot_mat->c2.z = 0;
  rot_mat->c2.w = 0;

  rot_mat->c3.x = -sin_theta;
  rot_mat->c3.y = 0;
  rot_mat->c3.z = cos_theta;
  rot_mat->c3.w = 0;

  rot_mat->c4.x = 0;
  rot_mat->c4.y = 0;
  rot_mat->c4.z = 0;
  rot_mat->c4.w = 1;

  return true;
}

bool rotate_mat4x4_z(fmat4x4_t* rot_mat, float theta) {
  if (rot_mat == NULL) return false;

  // Precompute trig values
  float sin_theta = sin(theta);
  float cos_theta = cos(theta);

  // Rotation
  rot_mat->c1.x = cos_theta;
  rot_mat->c1.y = sin_theta;
  rot_mat->c1.z = 0;
  rot_mat->c1.w = 0;

  rot_mat->c2.x = -sin_theta;
  rot_mat->c2.y = cos_theta;
  rot_mat->c2.z = 0;
  rot_mat->c2.w = 0;

  rot_mat->c3.x = 0;
  rot_mat->c3.y = 0;
  rot_mat->c3.z = 1;
  rot_mat->c3.w = 0;

  rot_mat->c4.x = 0;
  rot_mat->c4.y = 0;
  rot_mat->c4.z = 0;
  rot_mat->c4.w = 1;

  return true;
}

bool transform_mat4x4(fmat4x4_t* transform_mat, fmat4x4_t* translate_mat,
                      fmat4x4_t* rot_mat1, fmat4x4_t* rot_mat2,
                      fmat4x4_t* rot_mat3, fmat4x4_t* scale_mat) {
  if (transform_mat == NULL || translate_mat == NULL || rot_mat1 == NULL ||
      rot_mat2 == NULL || rot_mat3 == NULL || scale_mat == NULL) {
    return false;
  }

  fmat4x4_t temp1;
  fmat4x4_t temp2;

  return fmat4x4_matmul(translate_mat, rot_mat1, &temp1) &&
         fmat4x4_matmul(&temp1, rot_mat2, &temp2) &&
         fmat4x4_matmul(&temp2, rot_mat3, &temp1) &&
         fmat4x4_matmul(&temp1, scale_mat, transform_mat);
}
