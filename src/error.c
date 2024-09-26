#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "minishell.h"

// only the first error handler is implemented but not correctly.... the rest are just placeholders


void	print_error(char *cmd, char *msg)
{
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(msg, 2);
}

// Function to print command not found error
void	print_command_not_found(char *command)
{
	print_error(command, ": command not found\n");
	exit(127);  // Exit with code 127 when command is not found
}

// Function to print error for invalid exit arguments
void	print_exit_numeric_error(char *arg)
{
	print_error(arg, ": numeric argument required\n");
	exit(2);  // Exit with code 2 for non-numeric argument in exit
}
