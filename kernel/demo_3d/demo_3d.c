#include <mem.h>
#include <process.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

void _start() {
  // Press 'q' to exit the the program
  while (getc_silent(STD_IN) != 'q') {}
  exit();
  
  for (;;) {}
}
