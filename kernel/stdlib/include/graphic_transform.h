#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "math.h"


bool translate_mat4x4(fmat4x4_t* transl_mat, float dx, float dy, float dz);

bool scale_mat4x4(fmat4x4_t* scale_mat, float sx, float sy, float sz);

bool rotate_mat4x4(fmat4x4_t* rot_mat, float angle, fvec4_t* axis);

bool transform_mat4x4(fmat4x4_t* transform_mat, fmat4x4_t* translate_mat,
                      fmat4x4_t* rot_mat, fmat4x4_t* scale_mat);

bool perspective_mat4x4(fmat4x4_t* pers_mat, float screen_w, float screen_h,
                        float near, float far, float fov);