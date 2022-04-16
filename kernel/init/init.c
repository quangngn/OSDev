#include <mem.h>
#include <process.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "test_stdlib.h"

void _start() {
  // // Test functions for stdio.h and string.h
  // test_stdio();
  // test_string();

  char* s = NULL;
  size_t size = 0;
  uint64_t stream = STD_IN;
  printf("Please input something: \n");
  getline(&s, &size, &stream);

  printf("This is what you have type: \n");
  fprint_s(s);

  exit();
  for (;;) {
  }
}
