#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/******************************************************************************/
// 2D Vector and Matrix
typedef struct {
  int x;
  int y;
} ivec2_t;

typedef struct {
  float x;
  float y;
} fvec2_t;

// Column major matrix
typedef struct {
  ivec2_t c1;
  ivec2_t c2;
} imat2x2_t;

// Column major matrix
typedef struct {
  fvec2_t c1;
  fvec2_t c2;
} fmat2x2_t;

// Vec2 integer functions
// Elementwise add vectors
ivec2_t ivec2_add(ivec2_t v1, ivec2_t v2);
// Elementwise subtract vectors
ivec2_t ivec2_sub(ivec2_t v1, ivec2_t v2);
// Elementwise multiply vectors
ivec2_t ivec2_mult(ivec2_t v1, ivec2_t v2);
// Multiple vector with scalar
ivec2_t ivec2_scale(ivec2_t v1, int scale);
// Dot-product of two vectors
ivec2_t ivec2_dot_prod(ivec2_t v1, ivec2_t v2);
// Square of the vector's magnitude
int ivec2_mag_sq(ivec2_t v1);
// Check if two vectors are equal (x1 == x2 && y1 == y2)
bool ivec2_equal(ivec2_t v1, ivec2_t v2);

// Vec2 float functions
// Elementwise add vectors
fvec2_t fvec2_add(fvec2_t v1, fvec2_t v2);
// Elementwise subtract vectors
fvec2_t fvec2_sub(fvec2_t v1, fvec2_t v2);
// Elementwise multiply vectors
fvec2_t fvec2_ew_mult(fvec2_t v1, fvec2_t v2);
// Multiple vector with scalar
fvec2_t fvec2_scale(fvec2_t v1, float scale);
// Dot-product of two vectors
int fvec2_dot_prod(fvec2_t v1, fvec2_t v2);
// Square of the vector's magnitude
int fvec2_mag_sq(fvec2_t v1);
// Check if two vectors are equal (x1 == x2 && y1 == y2)
bool fvec2_equal(fvec2_t v1, fvec2_t v2);

// Matrix 2x2 functions
// Elementwise add two matrices
imat2x2_t imat2x2_add(imat2x2_t m1, imat2x2_t m2);
// Elementwise subtract two matrices
imat2x2_t imat2x2_sub(imat2x2_t m1, imat2x2_t m2);
// Multiply matrix with scale
imat2x2_t imat2x2_scale(imat2x2_t m1, int scale);
// Multiply two matrices
imat2x2_t imat2x2_matmul(imat2x2_t m1, imat2x2_t m2);
// Check if two matrices are equal
bool imat2x2_equal(imat2x2_t m1, imat2x2_t m2);

// Elementwise add two matrices
fmat2x2_t fmat2x2_add(fmat2x2_t m1, fmat2x2_t m2);
// Elementwise subtract two matrices
fmat2x2_t fmat2x2_sub(fmat2x2_t m1, fmat2x2_t m2);
// Multiply matrix with scale
fmat2x2_t fmat2x2_scale(fmat2x2_t m1, int scale);
// Multiply two matrices
fmat2x2_t fmat2x2_matmul(fmat2x2_t m1, fmat2x2_t m2);
// Check if two matrices are equal
bool fmat2x2_equal(fmat2x2_t m1, fmat2x2_t m2);

/******************************************************************************/
// 3D Vector and Matrix
typedef struct {
  int x;
  int y;
  int z;
} ivec3_t;

typedef struct {
  float x;
  float y;
  float z;
} fvec3_t;

// Column major matrix
typedef struct {
  ivec3_t c1;
  ivec3_t c2;
  ivec3_t c3;
} imat3x3_t;

// Column major matrix
typedef struct {
  fvec3_t c1;
  fvec3_t c2;
  fvec3_t c3;
} fmat3x3_t;

// Vec3 integer functions
// Elementwise add vectors
ivec3_t ivec3_add(ivec3_t v1, ivec3_t v2);
// Elementwise subtract vectors
ivec3_t ivec3_sub(ivec3_t v1, ivec3_t v2);
// Elementwise multiply vectors
ivec3_t ivec3_mult(ivec3_t v1, ivec3_t v2);
// Multiple vector with scalar
ivec3_t ivec3_scale(ivec3_t v1, int scale);
// Dot-product of two vectors
ivec3_t ivec3_dot_prod(ivec3_t v1, ivec3_t v2);
// Square of the vector's magnitude
int ivec3_mag_sq(ivec3_t v1);
// Check if two vectors are equal (x1 == x2 && y1 == y2 && z1 == z2)
bool ivec3_equal(ivec3_t v1, ivec3_t v2);

// Vec3 float functions
// Elementwise add vectors
fvec3_t fvec3_add(fvec3_t v1, fvec3_t v2);
// Elementwise subtract vectors
fvec3_t fvec3_sub(fvec3_t v1, fvec3_t v2);
// Elementwise multiply vectors
fvec3_t fvec3_ew_mult(fvec3_t v1, fvec3_t v2);
// Multiple vector with scalar
fvec3_t fvec3_scale(fvec3_t v1, float scale);
// Dot-product of two vectors
fvec3_t fvec3_dot_prod(fvec3_t v1, fvec3_t v2);
// Square of the vector's magnitude
int fvec3_mag_sq(fvec3_t v1);
// Check if two vectors are equal (x1 == x2 && y1 == y2 && z1 == z2)
bool fvec3_equal(fvec3_t v1, fvec3_t v2);

// Matrix 3x3 functions
// Elementwise add two matrices
imat3x3_t imat3x3_add(imat3x3_t m1, imat3x3_t m2);
// Elementwise subtract two matrices
imat3x3_t imat3x3_sub(imat3x3_t m1, imat3x3_t m2);
// Multiply matrix with scale
imat3x3_t imat3x3_scale(imat3x3_t m1, int scale);
// Multiply two matrices
imat3x3_t imat3x3_matmul(imat3x3_t m1, imat3x3_t m2);
// Check if two matrices are equal
bool imat3x3_equal(imat3x3_t m1, imat3x3_t m2);

// Elementwise add two matrices
fmat3x3_t fmat3x3_add(fmat3x3_t m1, fmat3x3_t m2);
// Elementwise subtract two matrices
fmat3x3_t fmat3x3_sub(fmat3x3_t m1, fmat3x3_t m2);
// Multiply matrix with scale
fmat3x3_t fmat3x3_scale(fmat3x3_t m1, int scale);
// Multiply two matrices
fmat3x3_t fmat3x3_matmul(fmat3x3_t m1, fmat3x3_t m2);
// Check if two matrices are equal
bool fmat3x3_equal(fmat3x3_t m1, fmat3x3_t m2);

/******************************************************************************/
// 4D Vector and Matrix
typedef struct {
  int x;
  int y;
  int z;
  int w;
} ivec4_t;

typedef struct {
  float x;
  float y;
  float z;
  float w;
} fvec4_t;

// Column major matrix
typedef struct {
  ivec4_t c1;
  ivec4_t c2;
  ivec4_t c3;
  ivec4_t c4;
} imat4x4_t;

// Column major matrix
typedef struct {
  fvec4_t c1;
  fvec4_t c2;
  fvec4_t c3;
  fvec4_t c4;
} fmat4x4_t;

// Vec4 integer functions
// Elementwise add vectors
ivec4_t ivec4_add(ivec4_t v1, ivec4_t v2);
// Elementwise subtract vectors
ivec4_t ivec4_sub(ivec4_t v1, ivec4_t v2);
// Elementwise multiply vectors
ivec4_t ivec4_mult(ivec4_t v1, ivec4_t v2);
// Multiple vector with scalar
ivec4_t ivec4_scale(ivec4_t v1, int scale);
// Dot-product of two vectors
int ivec4_dot_prod(ivec4_t v1, ivec4_t v2);
// Square of the vector's magnitude
int ivec4_mag_sq(ivec4_t v1);
// Check if two vectors are equal (x1 == x2 && y1 == y2 && z1 == z2 && w1 == w2)
bool ivec4_equal(ivec4_t v1, ivec4_t v2);

// Vec4 float functions
// Elementwise add vectors
fvec4_t fvec4_add(fvec4_t v1, fvec4_t v2);
// Elementwise subtract vectors
fvec4_t fvec4_sub(fvec4_t v1, fvec4_t v2);
// Elementwise multiply vectors
fvec4_t fvec4_ew_mult(fvec4_t v1, fvec4_t v2);
// Multiple vector with scalar
fvec4_t fvec4_scale(fvec4_t v1, float scale);
// Dot-product of two vectors
float fvec4_dot_prod(fvec4_t v1, fvec4_t v2);
// Square of the vector's magnitude
int fvec4_mag_sq(fvec4_t v1);
// Check if two vectors are equal (x1 == x2 && y1 == y2 && z1 == z2 && w1 == w2)
bool fvec4_equal(fvec4_t v1, fvec4_t v2);

// Matrix 4x4 functions
// Elementwise add two matrices
imat4x4_t imat4x4_add(imat4x4_t m1, imat4x4_t m2);
// Elementwise subtract two matrices
imat4x4_t imat4x4_sub(imat4x4_t m1, imat4x4_t m2);
// Multiply matrix with scale
imat4x4_t imat4x4_scale(imat4x4_t m1, int scale);
// Multiply two matrices
imat4x4_t imat4x4_matmul(imat4x4_t m1, imat4x4_t m2);
// Check if two matrices are equal
bool imat4x4_equal(imat4x4_t m1, imat4x4_t m2);

// Elementwise add two matrices
fmat4x4_t fmat4x4_add(fmat4x4_t m1, fmat4x4_t m2);
// Elementwise subtract two matrices
fmat4x4_t fmat4x4_sub(fmat4x4_t m1, fmat4x4_t m2);
// Multiply matrix with scale
fmat4x4_t fmat4x4_scale(fmat4x4_t m1, int scale);
// Multiply two matrices
fmat4x4_t fmat4x4_matmul(fmat4x4_t m1, fmat4x4_t m2);
// Check if two matrices are equal
bool fmat4x4_equal(fmat4x4_t m1, fmat4x4_t m2);

/******************************************************************************/
// Vector conversion (by add extra dimension 0)
// 2D to 3D and 4D
ivec3_t ivec2_to_ivec3(ivec2_t v);
ivec4_t ivec2_to_ivec4(ivec2_t v);
fvec3_t fvec2_to_fvec3(ivec2_t v);
fvec4_t fvec2_to_fvec4(ivec2_t v);

// 3D to 4D
ivec4_t ivec3_to_ivec4(ivec3_t v);
fvec4_t fvec3_to_fvec4(ivec3_t v);

// Int to Float
fvec2_t ivec2_itf(ivec2_t v);
fvec3_t ivec3_itf(ivec3_t v);
fvec4_t ivec4_itf(ivec4_t v);