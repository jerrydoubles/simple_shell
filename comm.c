#include "shell.h"

/**
 * handle_comments - handles comments passed as arguments
 * @line: line of code to be checked
 */
void handle_comments(char *line)
{
	int i = 0;

	while (line[i] != '\0')
	{
		if (line[i] == '#')
		{
			/* This is a comment. */
			i = strlen(line);
		}
		else
		{
			/* This is not a comment. */
			break;
		}
	}
}
