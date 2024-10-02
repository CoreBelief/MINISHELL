#include "builtins.h"
#include "minishell.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void	builtin_pwd(char **args, t_shell *shell);

void	builtin_pwd(char **args, t_shell *shell)
{
	char	*cwd;

	(void)args;
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		printf("%s\n", cwd);
		free(cwd);
		shell->last_exit_status=0;
	}
	else
	{
		perror("pwd");
		shell->last_exit_status=1;
	}
}