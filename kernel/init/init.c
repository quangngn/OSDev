#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

#define SYS_read 0
#define SYS_write 1


void _start() {
  // Issue a write system call
  printf("Hi, my name is %s, I am %d yo. My fav hex number is %x\n", "Quang", 22, 0x1234);

  // Loop forever
  for(;;){}
}
