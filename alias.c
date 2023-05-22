#include "shell.h"

void alias(char *line) {
  char *args[100];
  int num_args = split_commands(line, args);

  for (int i = 0; i < num_args; i++) {
    if (strcmp(args[i], "alias") == 0) {
      if (i + 1 == num_args) {
        // No aliases were specified.
        printf("No aliases specified.\n");
      } else {
        for (int j = i + 1; j < num_args; j++) {
          // Print the alias.
          printf("%s='%s'\n", args[j], getenv(args[j]));
        }
      }
    } else {
      // Define the alias.
      if (setenv(args[i], args[i + 1], 1) != 0) {
        perror("Error setting alias\n");
      }
    }
  }
}

