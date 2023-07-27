#include "shell.h"

/**
 * read_user_input - this function reads user input
 *
 * Return: A pointer to the string read.
 */
char *read_user_input(void)
{
	char *inp_str = NULL;
	size_t inp_size = 0;
	ssize_t inp_len;

	printf("Jeredi> ");
	inp_len = getline(&inp_str, &inp_size, stdin);

	if (inp_len == -1)
	{
		/* Handle end-of-file (Ctrl+D) or error */
		printf("\n");
		free(inp_str);
		return (NULL);
	}

	/* Remove newline character from input */
	while (inp_len > 0 && (inp_str[inp_len - 1] == '\n' ||
				inp_str[inp_len - 1] == ' ' ||
				inp_str[inp_len - 1] == '\t'))
	{
		inp_str[--inp_len] = '\0';
	}
	return (inp_str);
}

/**
 * tokenize_input - this function tokenize user input
 * @inp: pointer to the string read
 * @tokens: an array of pointers to the tokenized string
 *
 * Return: Number of arguments parsed.
 */
int tokenize_input(char *inp, char *tokens[])
{
	char *token;
	int argc = 0;

	token = strtok(inp, " \t");
	while (token != NULL)
	{
		tokens[argc++] = token;
		token = strtok(NULL, " \t");
	}

	return (argc);
}

/**
 * execute_cmd - this function executes a command
 * @cmd: command to execute
 * @args: array of strings to be passed to the command
 * @envp: environment variables that will be available to the executed command
 * @app: pointer to the initial app
 */
void execute_cmd(char *cmd, char *args[], char *envp[],
		char *app __attribute__((unused)))
{
	pid_t pid = fork();

	if (pid < 0)
		perror("Fork error");
	else if (pid == 0)
	{
		/* Child process */
		if (execve(cmd, args, envp) < 0)
		{
			/* Command execution failed */
			perror("./hsh");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		/* Parent process */
		int status;

		waitpid(pid, &status, 0);
	}
}

/**
 * main - Entry point
 * @ac: argument count
 * @av: argument vector
 *
 * Return: Always 0.
 */
int main(int ac __attribute__((unused)), char *av[])
{
	char *input;
	char *tokens[MAX_INPUT_LENGTH];
	int argc;
	char *envp[] = {
		"HOME=/home/user",
		"PATH=/usr/local/bin:/usr/bin:/bin",
		NULL
	};

	while (1)
	{
		input = read_user_input();
		if (input == NULL)
		{
			/* End of file or error */
			break;
		}

		if (strlen(input) == 0)
		{
			free(input);
			continue;
		}

		argc = tokenize_input(input, tokens);

		if (argc > 0)
		{
			/* Execute the command if no arguments are provided */
			if (argc == 1)
				execute_cmd(tokens[0], tokens, envp, av[0]);
			else
			{
				/* Display an error message if arguments are provided */
				printf("./hsh: No such file or directory\n");
			}
		}
		free(input);
	}
	return (0);
}
