#include "shell.h"

static char *lineptr = NULL;
static size_t n = 0;

ssize_t my_getline(char **lineptr) {
  ssize_t nread;

  while (1) {
    nread = read(0, lineptr, n);

    if (nread == -1) {
      if (errno == EINTR) {
        continue;
      } else {
        perror("Error reading from stdin\n");
        return -1;
      }
    } else if (nread == 0) {
      *lineptr = NULL;
      return 0;
    } else {
      break;
    }
  }

  for (size_t i = 0; i < nread; i++) {
    if (lineptr[i] == '\n') {
      lineptr[i] = '\0';
      break;
    }
  }

  return nread;
}

int main(void) {
  char *line = NULL;
  size_t nread;

  while (1) {
    printf("(jeshell$) ");

    nread = my_getline(&line);

    if (nread == -1) {
      printf("Closing shell...\n");
      return (-1);
    }

    // Split the command line into arguments.
    char *args[100];
    int argc = 0;

    for (size_t i = 0; i < nread; i++) {
      if (line[i] == ' ') {
        args[argc++] = line + i + 1;
        line[i] = '\0';
      }
    }

    args[argc++] = line;

    // Check if the command exists in the PATH.
    char *path = getenv("PATH");
    if (path == NULL) {
      perror("Error getting PATH environment variable\n");
      free(line);
      return (-1);
    }

    char *full_path = malloc(strlen(path) + strlen(args[0]) + 2);
    if (full_path == NULL) {
      perror("Error allocating memory for full path\n");
      free(line);
      return (-1);
    }

    sprintf(full_path, "%s/%s", path, args[0]);

    // Check if the command exists in the full path.
    int exists = access(full_path, F_OK);
    if (exists == -1) {
      perror("Command does not exist\n");
      free(line);
      free(full_path);
      return (-1);
    }

    // Execute the command.
    if (strcmp(args[0], "exit") == 0) {
      int status = 0;
      if (argc > 1) {
        status = atoi(args[1]);
      }
      free(line);
      free(full_path);
      exit(status);
    } else if (strcmp(args[0], "env") == 0) {
      free(line);
      free(full_path);
      char **envp = environ;
      while (*envp != NULL) {
        printf("%s=%s\n", *envp, *(envp + 1));
        envp += 2;
      }
      return (0);
    } else if (strcmp(args[0], "setenv") == 0) {
      if (argc != 3) {
        fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
        return 1;
      }
      if (setenv(args[1], args[2], 1) != 0) {
        perror("Error setting environment variable\n");
        return 1;
      }
    } else if (strcmp(args[0], "unsetenv") == 0) {
      if (argc != 2) {
        fprintf(stderr, "Usage: unsetenv VARIABLE\n");
        return 1;
      }
      if (unsetenv(args[1]) != 0) {
        perror("Error unsetting environment variable\n");
        return 1;
      }
    } else if (strcmp(args[0], "cd") == 0) {
      if (argc == 1) {
        // Change to the user's home directory.
        char *home = getenv("HOME");
        if (home == NULL) {
          perror("Error getting HOME environment variable\n");
          return 1;
        }
        if (chdir(home) != 0) {
          perror("Error changing directory\n");
          return 1;
        }
      } else if (argc == 2) {
        // Change to the specified directory.
        if (chdir(args[1]) != 0) {
          perror("Error changing directory\n");
          return 1;
        }
      } else {
        fprintf(stderr, "Usage: cd [DIRECTORY]\n");
        return 1;
      }
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

    free(line);
    free(full_path);
  }

  return (0);
}
