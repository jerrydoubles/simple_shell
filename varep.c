#include "shell.h"

void expand_variables(char *line) {
  char *args[100];
  int num_args = split_commands(line, args);

  for (int i = 0; i < num_args; i++) {
    if (args[i][0] == '$') {
      // Replace the variable with its value.
      char *var_name = args[i] + 1;
      char *var_value = getenv(var_name);
      if (var_value == NULL) {
        // The variable is not defined.
        args[i] = NULL;
      } else {
        // Replace the variable with its value.
        args[i] = var_value;
      }
    }
  }
}

