#include "shell.h"

/**
 * main_shell_loop - Main shell loop that keeps running until the user exits
 * @info: Pointer to the shell info struct
 * @argv: Array of strings containing command-line arguments
 *
 * Return: Returns the status of the last executed built-in command
 */
int main_shell_loop(info_t *info, char **argv)
{
	int builtin_result = 0;
	char *input_buffer = NULL;
	size_t buffer_size = 0;

	while (1)
	{
		clear_shell_info(info);

		if (is_interactive(info))
			print_prompt();

		flush_buffer();

		if (get_user_input(info, &input_buffer, &buffer_size) == -1)
			break;

		set_info_from_input(info, argv, input_buffer);
		builtin_result = find_and_execute_builtin(info);

		if (builtin_result == -1)
			find_and_execute_external_cmd(info);

		free_shell_info(info, 0);
		free(input_buffer);
	}

	write_shell_history(info);
	free_shell_info(info, 1);
	return (builtin_result);
}

/**
 * find_and_execute_builtin - Find and execute a built-in command
 * @info: Pointer to the shell info struct
 *
 * Return: Returns the status of the executed built-in command,
 *         or -1 if the command is not a built-in.
 */
int find_and_execute_builtin(info_t *info)
{
	static builtin_table builtins[] = {
		{"exit", execute_exit},
		{"env", execute_env},
		{"help", execute_help},
		{"history", execute_history},
		{"setenv", execute_setenv},
		{"unsetenv", execute_unsetenv},
		{"cd", execute_cd},
		{"alias", execute_alias},
		{NULL, NULL}
	};

	for (int i = 0; builtins[i].type; i++)
	{
		if (are_strings_equal(info->argv[0], builtins[i].type))
		{
			info->line_count++;
			return (builtins[i].func(info));
		}
	}
	return (-1);
}

/**
 * find_and_execute_external_cmd - Find and execute an external command
 * @info: Pointer to the shell info struct
 */
void find_and_execute_external_cmd(info_t *info)
{
	char *cmd_path = NULL;
	int arg_count = 0;

	info->path = info->argv[0];
	env_var = get_env_var(info, "PATH=");


	if (info->linecount_flag == 1)
	{
		info->line_count++;
		info->linecount_flag = 0;
	}

	for (int i = 0; info->arg[i]; i++)
	{
		if (!is_delimiter(info->arg[i], " \t\n"))
			arg_count++;
	}
	if (!arg_count)
		return;

	cmd_path = find_command_path(info, env_var, info->argv[0]);
	if (cmd_path)
	{
		info->path = cmd_path;
		fork_and_execute_cmd(info);
	}
	else
	{
		if ((is_interactive(info) || get_env_var(info, "PATH=") ||
					info->argv[0][0] == '/') && is_valid_cmd(info, info->argv[0]))
		{
			fork_and_execute_cmd(info);
		}
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			print_error(info, "Command not found\n");
		}
	}
}

/**
 * fork_and_execute_cmd - Fork and execute a command
 * @info: Pointer to the shell info struct
 */
void fork_and_execute_cmd(info_t *info)
{
	pid_t child_pid = fork();

	if (child_pid == -1)
	{
		perror("Error:");
		return;
	}

	if (child_pid == 0)
	{
		if (execute_external_cmd(info) == -1)
		{
			free_shell_info(info, 1);

			if (errno == EACCES)
				exit(126);
			exit(1);
		}
	}
	else
	{
		wait_for_child_process(&(info->status));
		if (child_process_exited_normally(info->status))
		{
			info->status = extract_exit_status(info->status);
			if (info->status == 126)
				print_error(info, "Permission denied\n");
		}
	}
}

/**
 * main - entry point of the application
 * @argc: number of arguments passed to the program
 * @argv: array of arguments
 *
 * Return: Always 0. Otherwise -1
 */
int main(int argc, char **argv)
{
	info_t shell_info = initialize_shell_info();
	int result = main_shell_loop(&shell_info, argv);

	return (result);
}
