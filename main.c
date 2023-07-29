#include "shell.h"

/**
 * read_user_input - this function reads user input
 * @interactive_mode: indicates what mode the program is being run in
 *
 * Return: A pointer to the string read.
 */
char *read_user_input(int interactive_mode)
{
	char *inp_str = NULL, *prompt = "Jeredi$ ";
	size_t inp_size = 0;
	ssize_t inp_len;

	if (interactive_mode)
		write(STDIN_FILENO, prompt, 8);
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
void execute_cmd(char *cmd, char *args[], char *envp[], char *app)
{
	struct stat file_stat;
	pid_t pid;
	int argc = 0;

	while (args[argc] != NULL)
		argc++;
	pid = fork();
	if (pid == -1)
	{
		perror("Fork error");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		/* Child process */
		if (argc > 1)
		{
			print_error(app, ": No such file or directory\n");
			exit(EXIT_FAILURE);
		}
		if (stat(cmd, &file_stat) == -1)
		{
			perror(app);
			exit(EXIT_FAILURE);
		}

		if (execve(cmd, args, envp) < -1)
		{
			/* Command execution failed */
			perror(app);
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
	int interactive_mode = 1;

	while (1 && interactive_mode)
	{
		interactive_mode = isatty(STDIN_FILENO);
		input = read_user_input(interactive_mode);
		if (input == NULL)
		{
			/* End of file or error */
			break;
		}

		if (_strlen(input) == 0)
		{
			free(input);
			continue;
		}

		argc = tokenize_input(input, tokens);

		if (argc > 0)
		{
			/* Execute the command if no arguments are provided */
			execute_cmd(tokens[0], tokens, envp, av[0]);
		}
		free(input);
	}
	return (0);
}
