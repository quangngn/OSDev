#pragma once

#include "graphic.h"

static const triangle_t std_right_tri = {
  .p0 = {.x = 0, .y = 0, .z = 0, .w = 1},
  .p1 = {.x = 0, .y = 1, .z = 0, .w = 1},
  .p2 = {.x = 1, .y = 0, .z = 0, .w = 1},
};

static const triangle_t std_tri = {
  .p0 = {.x = 0, .y = 0, .z = 0, .w = 1},
  .p1 = {.x = 0.5, .y = 0.5, .z = 0, .w = 1},
  .p2 = {.x = 0, .y = 1, .z = 0, .w = 1},
};


