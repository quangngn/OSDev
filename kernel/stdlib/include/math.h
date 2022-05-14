#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "trigonometry.h"
#include "vec.h"

static inline int abs(int v) { return v < 0 ? -v : v; }

static inline float absf(float v) { return v < 0 ? -v : v; }

static inline int min(int v1, int v2) { return v1 < v2 ? v1 : v2; }

static inline int max(int v1, int v2) { return v1 > v2 ? v1 : v2; }

static inline float minf(float v1, float v2) { return v1 < v2 ? v1 : v2; }

static inline float maxf(float v1, float v2) { return v1 > v2 ? v1 : v2; }

static bool swap(int *x, int *y) {
  if (x == NULL || y == NULL) return false;

  int temp = *x;
  *x = *y;
  *y = temp;

  return true;
}

static bool swapf(float *x, float *y) {
  if (x == NULL || y == NULL) return false;

  float temp = *x;
  *x = *y;
  *y = temp;

  return true;
}

static inline float z_line_itpl(float a, float a0, float a1, float z0,
                                  float z1) {
  return (a - a0) * (z1 - z0) / (a1 - a0) + z0;
}