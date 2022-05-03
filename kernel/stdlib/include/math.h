#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "trigonometry.h"
#include "vec.h"

static inline int64_t abs(int64_t v) {
  return v < 0 ? -v : v;
}

static inline float absf(float v) {
  return v < 0 ? -v : v;
}

static inline bool get_sign(int64_t v) {
  return v >= 0;
}

static inline bool get_signf(float v) {
  return v >= 0;
}

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