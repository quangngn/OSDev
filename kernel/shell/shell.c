#include <mem.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <process.h>

void _start() {
  char* command = NULL;
  size_t command_buff_size = 0;
  uint64_t stream = STD_IN;

  // Endless loop of read to 
  do {
    // print prompt
    printf("> ");

    // Get input command
    getline(&command, &command_buff_size, &stream);

    // Extract exe_name from the input command
    char* exe_name = strsep(&command, " \n");
    while (strcmp(exe_name, "") == 0) exe_name = strsep(&command, " \n");

    // Execute the command
    exec(exe_name);
  } while (command == NULL || strcmp(command, "q") != 0);

  // Loop forever
  for (;;) {}
}
