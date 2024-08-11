#include "builtins.h"
#include "minishell.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	builtin_cd(char **args)
{
	char	*path;
	char	*oldpwd;
	char	cwd[1024];

	oldpwd = getcwd(NULL, 0);
	if (!args[1] || strcmp(args[1], "~") == 0)
		path = getenv("HOME");
	else if (ft_strcmp(args[1], "-") == 0)
		path = getenv("OLDPWD");
	else
		path = args[1];
	if (chdir(path) != 0)
	{
		perror("cd");
		free(oldpwd);
		return ;
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		setenv("OLDPWD", oldpwd, 1);
		setenv("PWD", cwd, 1);
	}
	else
		perror("getcwd");
	free(oldpwd);
}