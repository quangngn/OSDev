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

// Prompt the user for input through keyboard and then print out the exact
// string
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
  // Get the user input
  get_input();

  // Press 'q' to exit the the program
  printf("Finish execution, please press 'q' to exit the program!\n");
  while (getc_silent(STD_IN) != 'q') {
  }
  exit();

  for (;;) {
  }
}
