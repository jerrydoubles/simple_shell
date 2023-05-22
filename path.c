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

    // Check if the command exists in the PATH.
    char *path = getenv("PATH");
    if (path == NULL) {
      perror("Error getting PATH environment variable\n");
      free(lineptr);
      free(copied_lineptr);
      return (-1);
    }

    char *full_path = malloc(strlen(path) + strlen(args[0]) + 2);
    if (full_path == NULL) {
      perror("Error allocating memory for full path\n");
      free(lineptr);
      free(copied_lineptr);
      free(full_path);
      return (-1);
    }

    sprintf(full_path, "%s/%s", path, args[0]);

    // Check if the command exists in the full path.
    int exists = access(full_path, F_OK);
    if (exists == -1) {
      perror("Command does not exist\n");
      free(lineptr);
      free(copied_lineptr);
      free(full_path);
      return (-1);
    }

    // Execute the command.
    if (strcmp(args[0], "exit") == 0) {
      free(lineptr);
      free(copied_lineptr);
      free(full_path);
      return (0);
    } else {
      int pid = fork();
      if (pid == 0) {
        execvp(full_path, args);
        perror("Error executing command\n");
        exit(1);
      } else {
        wait(NULL);
      }
    }

    free(lineptr);
    free(copied_lineptr);
    free(full_path);
    return (0);
  }
}

