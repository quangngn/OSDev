#include "assert.h"

// Assert function:
int assert(bool expr, const char* errormsg) {
  if (!expr) {
    perror(errormsg);
    return 0;
  } else {
    return 1;
  }
}