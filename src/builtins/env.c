#include "builtins.h"
#include "minishell.h"
#include <stdio.h>

extern char **environ;
//this is a global variable that is defined in the main.c file, so its forbidden!!


void	builtin_env(char **args)
{
	int	i;

	(void)args;
	i = 0;
	while (environ[i])
	{
		printf("%s\n", environ[i]);
		i++;
	}
}