#include "builtins.h"
#include "minishell.h"
#include <stdio.h>

void	builtin_env(char **args, t_shell *shell);

void	builtin_env(char **args, t_shell *shell)
{
	int	i;

	(void)args;
	i = 0;
	while (shell->env[i])
	{
		printf("%s\n", shell->env[i]);
		i++;
	}
}
//fucking validation bitches
//with mothafucking exit codes