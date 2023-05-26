#include "shell.h"

/**
 * split_commands - splits a line into commands
 * @line: the line of command
 * @commands: array of commands
 *
 * Return: number of commands
 */
void split_commands(char *line, char **commands)
{
	int i = 0;
	int j = 0;

	while (line[i] != '\0')
	{
		if (line[i] == ';')
		{
			commands[j++] = line + i + 1;
			line[i] = '\0';
		}
		i++;
	}
	commands[j] = NULL;
}
