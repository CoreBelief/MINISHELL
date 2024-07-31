#include "builtins.h"
#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

void	builtin_exit(char **args)
{
	int	exit_status;

	printf("exit\n");
	if (!args[1])
	{
		exit(0);
	}
	exit_status = atoi(args[1]);
	if (args[2])
	{
		fprintf(stderr, "minishell: exit: too many arguments\n");
		return ;
	}
	if (exit_status == 0 && args[1][0] != '0')
	{
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n", args[1]);
		exit(255);
	}
	exit(exit_status & 255);
}