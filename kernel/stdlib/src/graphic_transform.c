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

bool transform_mat4x4(fmat4x4_t* transform_mat, fmat4x4_t* translate_mat,
                      fmat4x4_t* rot_mat, fmat4x4_t* scale_mat) {
  if (transform_mat == NULL || translate_mat == NULL || rot_mat == NULL ||
      scale_mat == NULL) {
    return false;
  }

  fmat4x4_t temp;
  return fmat4x4_matmul(translate_mat, rot_mat, &temp) &&
         fmat4x4_matmul(&temp, scale_mat, transform_mat);
}

bool perspective_mat4x4(fmat4x4_t* pers_mat, float screen_w, float screen_h,
                        float near, float far, float fov) {
  if (pers_mat == NULL) return 0;

  float tan_half_fov = tan(fov / 2);
  float aspect_ratio = screen_w / screen_h;

  float x = 1 / (aspect_ratio * tan_half_fov);
  float y = 1 / tan_half_fov;
  float z = (far + near) / (near - far);
  float w = (2 * far * near) / (near - far);

  pers_mat->c1.x = x;
  pers_mat->c1.y = 0;
  pers_mat->c1.z = 0;
  pers_mat->c1.w = 0;

  pers_mat->c2.x = 0;
  pers_mat->c2.y = y;
  pers_mat->c2.z = 0;
  pers_mat->c2.w = 0;

  pers_mat->c3.x = 0;
  pers_mat->c3.y = 0;
  pers_mat->c3.z = z;
  pers_mat->c3.w = -1;

  pers_mat->c4.x = 0;
  pers_mat->c4.y = 0;
  pers_mat->c4.z = w;
  pers_mat->c4.w = 0;

  return true;
}