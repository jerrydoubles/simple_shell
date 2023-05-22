#include "shell.h"

/**
 * main - entry point of the application
 * @ac: arguments count
 * @av: argument vector
 *
 * Return: Always 0.
 */
int main(void)
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

                copied_lineptr = malloc((n + 1) * sizeof(char));
                if (copied_lineptr == NULL)
                {
                        free(copied_lineptr);
                        free(lineptr);
                        perror("Error occured while running malloc\n");
                        return (-1);
                }

                strcpy(copied_lineptr, lineptr);
                copied_lineptr[n] = '\0';
                printf(">>debug<<: %s\n", copied_lineptr);
        }
        free(lineptr);
        free(copied_lineptr);

	int exit_status = WEXITSTATUS(status);
	printf("The exit status of the last command was %d.\n", exit_status);
	printf("The process ID of this shell is %d.\n", getpid());

	return (0);
}
