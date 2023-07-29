#include "shell.h"

/**
 * initialize_shell_info - initializes shell info
 *
 * Return: initial info
 */
info_t initialize_shell_info(void)
{
	info_t shell_info;

	shell_info.arg = NULL;
	shell_info.argv = NULL;
	shell_info.path = NULL;
	shell_info.argc = 0;
	shell_info.line_count = 0;
	shell_info.err_num = 0;
	shell_info.linecount_flag = 0;
	shell_info.fname = NULL;
	shell_info.environ = NULL;
	shell_info.env_changed = 0;
	shell_info.status = 0;
	shell_info.readfd = 0;

	return (shell_info);
}

/**
 * print_error - Prints error messages
 * @app: name of the shell program
 * @message: message to print out
 */
void print_error(const char *app, const char *message)
{
	size_t app_len = strlen(app);
	size_t message_len = strlen(message);

	write(STDERR_FILENO, app, app_len);
	write(STDERR_FILENO, message, message_len);
}
