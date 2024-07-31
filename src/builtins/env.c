#include "builtins.h"
#include "minishell.h"
#include <stdio.h>

extern char **environ;

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