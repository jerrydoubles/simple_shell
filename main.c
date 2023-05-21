#include "shell.h"

/**
 * main - entry point of the application
 * @ac: arguments count
 * @av: argument vector
 *
 * Return: Always 0.
 */
int main(int ac __attribute__((unused)), char *av[] __attribute__((unused)))
{
	char *lineptr = NULL, *copied_lineptr = NULL;
	size_t n = 0;
	ssize_t nread;

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

		copied_lineptr = malloc((nread + 1) * sizeof(char));
		if (copied_lineptr == NULL)
		{
			free(copied_lineptr);
			free(lineptr);
			perror("Error occured while running malloc\n");
			return (-1);
		}

		strcpy(copied_lineptr, lineptr);
		printf(">>debug<<: %s\n", copied_lineptr);
	}

	free(lineptr);
	free(copied_lineptr);
	return (0);
}
