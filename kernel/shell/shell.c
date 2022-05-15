#include <mem.h>
#include <process.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

void _start() {
  char* line = NULL;
  size_t line_size = 0;
  uint64_t stream = STD_IN;

  // Endless loop of read to
  while (true) {
    printf("> ");
    if (getline(&line, &line_size, &stream) == -1) {
      perror("[ERROR] Unable to read command line.\n");
    } else {
      // Read toke from line. The first valid token (non null, non empty) is our
      // executable's name.
      char* tok = strtok(line, " \n");
      while (strcmp(tok, "") == 0 && tok != NULL) {
        tok = strtok(NULL, " \n");
      }

      // If type "quit", the shell program exits, else we launch the executable.
      if (strcmp(tok, "clear") == 0) {
        exec("shell");
      } else {
        exec(tok);
      }
    }
    free(line);
    line = NULL;
    line_size = 0;
  }

  // Loop forever
  for (;;) {
  }
}
