#include "shell.h"

int main(void) {
  char *lineptr = NULL, *copied_lineptr = NULL;
  size_t n = 0;
  ssize_t nread;

  while (1) {
    printf("(jeshell$) ");

    nread = getline(&lineptr, &n, stdin);

    if (nread == -1) {
      free(lineptr);
      free(copied_lineptr);
      printf("Closing shell...\n");
      return (-1);
    }

    copied_lineptr = malloc((n + 1) * sizeof(char));
    if (copied_lineptr == NULL) {
      free(copied_lineptr);
      free(lineptr);
      perror("Error occured while running malloc\n");
      return (-1);
    }

    strcpy(copied_lineptr, lineptr);
    copied_lineptr[n] = '\0';

    // Split the command line into arguments.
    char *args[100];
    int argc = 0;
    args[argc++] = strtok(copied_lineptr, " ");
    while (args[argc - 1] != NULL) {
      args[argc++] = strtok(NULL, " ");
    }

    // Execute the command.
    if (strcmp(args[0], "exit") == 0) {
      free(lineptr);
      free(copied_lineptr);
      return (0);
    } else {
      execvp(args[0], args);
      perror("Error executing command\n");
      free(lineptr);
      free(copied_lineptr);
      return (-1);
    }
  }

  free(lineptr);
  free(copied_lineptr);
  return (0);
}

