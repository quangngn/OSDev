#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "math.h"

/**
 * Construct a translation matrix based on given movement in x, y, and z
 * direction. The result is stored in transl_mat.
 * \returns true if succeed, false other wise.
 */
bool translate_mat4x4(fmat4x4_t* transl_mat, float dx, float dy, float dz);

/**
 * Construct a scaling matrix based on given scale of in x, y, and z direction.
 * The result is stored in scale_mat.
 * \returns true if succeed, false otherwise.
 */
bool scale_mat4x4(fmat4x4_t* scale_mat, float sx, float sy, float sz);

/**
 * Source: https://en.wikipedia.org/wiki/Rotation_matrix
 * 
 * Construct a rotation matrix based on given the angle and an axis. The axis
 * must be a unit vector. The result is stored in rot_mat.
 * \returns true if succeed, false otherwise.
 */
bool rotate_mat4x4(fmat4x4_t* rot_mat, float angle, fvec4_t* axis);

/**
 * Construct a general transformation matrix from the given translation,
 * rotation, and scale matrix. The order is as follows:
 * transf = transl x rotation x scale
 * \returns true if succeed, false otherwise.
 */
bool transform_mat4x4(fmat4x4_t* transform_mat, fmat4x4_t* translate_mat,
                      fmat4x4_t* rot_mat, fmat4x4_t* scale_mat);