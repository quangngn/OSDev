#include "time.h"

uint64_t get_time() {
  uint64_t msr;
  __asm__("rdtsc\n" : "=a"(msr));
  return msr;
}