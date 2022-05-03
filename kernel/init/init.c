#include <mem.h>
#include <process.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "test_stdlib.h"

void run_test() {
  // Test functions for stdio.h and string.h
  // test_stdio();
  // test_string();
  // test_trig();
}

void get_input() {
  char* s = NULL;
  size_t size = 0;
  uint64_t stream = STD_IN;
  printf("Please input something: \n");
  getline(&s, &size, &stream);
  printf("This is what you have typed: \n");
  fprint_s(STD_OUT, s);
}

void _start() {
  // get_input();
  run_test();

  exit();
  for (;;) {}
}
