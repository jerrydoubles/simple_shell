#include "shell.h"

void execute_file(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("Error opening file\n");
    return;
  }

  char line[1000];
  while (fgets(line, 1000, fp) != NULL) {
    // Strip the newline character from the end of the line.
    line[strlen(line) - 1] = '\0';

    // Handle comments.
    handle_comments(line);

    // If the line is not empty, execute the command.
    if (line[0] != '\0') {
      execute_command(line);
    }
  }

  fclose(fp);
}

