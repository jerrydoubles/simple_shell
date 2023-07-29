#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

#define READ_BUF_SIZE 1024
#define MAX_INPUT_LENGTH 1024

/**
 * struct passinfo - contains passed in info
 * @arg: the argument passed in
 * @argv: the argument vector
 * @path: the pointer to the path
 * @argc: the number of arguments
 * @line_count: the parameter
 * @err_num: error numbers
 * @linecount_flag: the line count flag
 * @fname: the file name
 * @environ: a pointer to an array of strings reps env var
 * @env_changed: indicates a change in environment variable
 * @status: the exist status
 * @readfd: the file descriptor
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	char **environ;
	int env_changed;
	int status;
	int readfd;
} info_t;

/**
 * struct builtin - contains a builtin string and related function
 * @type: the builtin command flag
 * @func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;

int hsh(info_t *, char **);
int find_builtin(info_t *);
void find_cmd(info_t *);
void fork_cmd(info_t *);
int is_cmd(info_t *, char *);
info_t initialize_shell_info(void);
void print_error(char *app, char *message);
int _strlen(char *s);

#endif
