#include "shell.h"

void handle_comments(char *line) {
  int i = 0;

  while (line[i] != '\0') {
    if (line[i] == '#') {
      // This is a comment.
      i = strlen(line);
    } else {
      // This is not a comment.
      break;
    }
  }
}

