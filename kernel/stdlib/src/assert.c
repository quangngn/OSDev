#include "assert.h"

/**
 * Print out the error message to STD error if expr is false.
 * \param expr Boolean expression.
 * \param errormsg Message to be printed if expr is false.
 * \returns 1 if expr is true, and 0 if expr is false
 */ 
int assert(bool expr, const char* errormsg) {
  if (!expr) {
    perror(errormsg);
    return 0;
  } else {
    return 1;
  }
}