#include <mem.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <process.h>

#include "test_stdlib.h"

void _start() {
  test_stdio();
  test_string();
  
  exit();
  for(;;){}
}
