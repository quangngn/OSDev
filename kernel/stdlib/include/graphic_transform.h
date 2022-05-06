#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "math.h"

typedef struct {
  float sx;
  float sy;
  float sz;
  float dx;
  float dy;
  float dz;
  float degx;
  float degy;
  float degz;
} transform_info_t;

bool translate_mat4x4(fmat4x4_t* transl_mat, float dx, float dy, float dz);

bool scale_mat4x4(fmat4x4_t* scale_mat, float sx, float sy, float sz);

bool rotate_mat4x4(fmat4x4_t* rot_mat, float degx, float degy, float degz);

bool transform_mat4x4(fmat4x4_t* transform_mat, fmat4x4_t* translate_mat,
                      fmat4x4_t* rot_mat1, fmat4x4_t* rot_mat2,
                      fmat4x4_t* rot_mat3, fmat4x4_t* scale_mat);

bool perspective_proj_mat4x4(float near, float far, float angle);