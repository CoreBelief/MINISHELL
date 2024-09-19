#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

// only the first error handler is implemented but not correctly.... the rest are just placeholders


// Function to print command not found error
void	print_command_not_found(char *command)
{
	fprintf(stderr, "%s: command not found\n", command);
	exit(127);  // Exit with code 127 when command is not found
}

// Function to print error for invalid exit arguments
void	print_exit_numeric_error(char *arg)
{
	fprintf(stderr, "exit: %s: numeric argument required\n", arg);
	exit(2);  // Exit with code 2 for non-numeric argument in exit
}

// Function to handle unset PATH error
void	print_unset_path_error(void)
{
	fprintf(stderr, "error: PATH is unset\n");
	exit(1);  // Exit with code 1 if PATH is unset
}

// Function to handle file/directory not found errors
void	print_file_not_found_error(char *path)
{
	fprintf(stderr, "%s: No such file or directory\n", path);
	exit(errno);  // Use errno for error code
}

// General function for handling permission errors
void	print_permission_error(char *path)
{
	fprintf(stderr, "%s: Permission denied\n", path);
	exit(126);  // Exit with code 126 for permission denied
}

// Function for handling syntax errors (like unexpected tokens)
void	print_syntax_error(char *token)
{
	fprintf(stderr, "syntax error near unexpected token `%s`\n", token);
	exit(258);  // Exit with code 258 for syntax errors
}

// Function to print generic errors
void	print_generic_error(char *msg)
{
	fprintf(stderr, "%s\n", msg);
	exit(1);
}

// Function for handling memory allocation errors
void	handle_memory_error(void)
{
	fprintf(stderr, "error: Memory allocation failed\n");
	exit(1);  // Exit with code 1 for memory allocation failures
}

// You can add more error handling functions based on specific project requirements

