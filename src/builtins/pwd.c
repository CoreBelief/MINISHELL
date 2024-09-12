#include "builtins.h"
#include "minishell.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void	builtin_pwd(char **args)
{
	char	*cwd;

	(void)args;
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		printf("%s\n", cwd);
		free(cwd);
	}
	else
	{
		perror("pwd");
	}
}