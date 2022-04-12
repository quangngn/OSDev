#include <mem.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "test_stdlib.h"

int sum(int* arr, size_t len) {
  int sum = 0;
  for (int i = 0; i < len; i++) {
    sum += arr[i];
  }
  return sum;
}

void _start() {
  test_stdio();

  // Loop forever
  for (;;) {}
}
