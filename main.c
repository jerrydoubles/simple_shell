#include "shell.h"

/**
 * executeCommand - Executes a command and retrieves its exit status.
 *
 * @command: The command to be executed.
 *
 * Return: The exit status of the command.
 */
void execvpWrapper(char *command);

int executeCommand(char *command)
{
	pid_t child_pid = fork();

	if (child_pid == -1)
	{
	perror("Error occurred while forking\n");
	return (-1);
	}
	else if (child_pid == 0)
	{
		execvpWrapper(command);
		exit(1);
	}
	else
	{
		int status;

		waitpid(child_pid, &status, 0);
		return (WEXITSTATUS(status));
	}
}
/**
 * execvpWrapper - Calls execvp with the command and handles memory allocation.
 *
 * @command: The command to be executed.
 */
void execvpWrapper(char *command)
{
	char **args = NULL;
	char *token = strtok(command, " \t\n");
	int numArgs = 0;

	while (token != NULL)
	{
		args = realloc(args, (numArgs + 2) * sizeof(char *));
		if (args == NULL)
		{

			perror("Error occurred while executing command\n");
			exit(1);
		}
		args[numArgs] = strdup(token);
		if (args[numArgs] == NULL)
		{
			perror("Error occurred while duplicating string\n");
			exit(1);
		}
		numArgs++;
		token = strtok(NULL, " \t\n");
	}
	args[numArgs] = NULL;
	execvp(args[0], args);
	perror("Error occurred while executing command\n");
	exit(1);
}
/**
 * main - Entry point of the application.
 *
 * Return: Always 0.
 */
int main(void)
{
	char *lineptr = NULL, *copied_lineptr = NULL;
	size_t n = 0;
	ssize_t nread;
	int exit_status = 0;

	while (1)
	{
	printf("(jeshell$) ");
	nread = getline(&lineptr, &n, stdin);

	if (nread == -1)
	{
		free(lineptr);
		free(copied_lineptr);
		printf("Closing shell...\n");
	return (-1);
	}

	copied_lineptr = malloc((n + 1) * sizeof(char));
	if (copied_lineptr == NULL)
	{
		free(copied_lineptr);
		free(lineptr);
		perror("Error occurred while running malloc\n");
		return (-1);
	}
	strcpy(copied_lineptr, lineptr);
	copied_lineptr[n] = '\0';
	printf(">>debug<<: %s\n", copied_lineptr);

	exit_status = executeCommand(copied_lineptr);

	printf("The exit status of the last command was %d.\n", exit_status);
	printf("The process ID of this shell is %d.\n", getpid());
	}

	free(lineptr);
	free(copied_lineptr);

	return (0);
}
