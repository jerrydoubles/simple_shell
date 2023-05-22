#include "shell.h"

void parse_commands(char *line, char **commands) {
  int i = 0;
  int j = 0;

  while (line[i] != '\0') {
    if (line[i] == '&' && line[i + 1] == '&') {
      commands[j++] = line + i + 2;
      line[i] = '\0';
      i++;
    } else if (line[i] == '|' && line[i + 1] == '|') {
      commands[j++] = line + i + 2;
      line[i] = '\0';
      i++;
    }
    i++;
  }

  commands[j] = NULL;
}
while (1) {
  printf("(jeshell$) ");

  nread = my_getline(&line);

  if (nread == -1) {
    printf("Closing shell...\n");
    return (-1);
  }

  char *commands[100];
  int num_commands = parse_commands(line, commands);

  for (int i = 0; i < num_commands; i++) {
    // Execute the command.
    if (strcmp(commands[i], "exit") == 0) {
      int status = 0;
      if (argc > 1) {
        status = atoi(args[1]);
      }
      free(line);
      free(full_path);
      exit(status);
    } else if (strcmp(commands[i], "env") == 0) {
      free(line);
      free(full_path);
      char **envp = environ;
      while (*envp != NULL) {
        printf("%s=%s\n", *envp, *(envp + 1));
        envp += 2;
      }
      return (0);
    } else if (strcmp(commands[i], "setenv") == 0) {
      if (argc != 3) {
        fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
        return 1;
      }
      if (setenv(args[1], args[2], 1) != 0) {
        perror("Error setting environment variable\n");
        return 1;
      }
    } else if (strcmp(commands[i], "unsetenv") == 0) {
      if (argc != 2) {
        fprintf(stderr, "Usage: unsetenv VARIABLE\n");
        return 1;
      }
      if (unsetenv(args[1]) != 0) {
        perror("Error unsetting environment variable\n");
        return 1;
      }
    } else if (strcmp(commands[i], "cd") == 0) {
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
      int pid = fork();
      if (pid == 0) {
        if (line[i] == '&' && line[i + 1] == '&') {
          // Execute the next command only if the previous command succeeded.
          execvp(commands[i], NULL);
          if (WEXITSTATUS(status) != 0) {
            exit(WEXITSTATUS(status));
          }
        } else if (line[i] == '|' && line[i + 1] == '|') {
          // Execute the next command only if the previous command failed.
          execvp(commands[i], NULL);
          if (WEXITSTATUS(status) == 0) {
            exit(WEXITSTATUS(status));
          }
        }
      } else {
        wait(NULL);
      }
    }
  }

  free(line);
  free(full_path);
}
	return (0);
}
