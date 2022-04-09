#include <mem.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

int sum(int* arr, size_t len) {
  int sum = 0;
  for (int i = 0; i < len; i++) {
    sum += arr[i];
  }
  return sum;
}

void _start() {
  // Issue a write system call
  printf("Hi, my name is %s, I am %d yo. My fav hex number is %p\n", "Quang",
         22, 0x232433);

  size_t len = 50000;
  int* arr = (int*)malloc(sizeof(int) * len);
  for (int i = 0; i < len; i++) {
    arr[i] = i;
  }

  printf("Sum is %d\n", sum(arr, len));

  // Loop forever
  for (;;) {}
}
