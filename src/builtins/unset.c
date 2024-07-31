#include "builtins.h"
#include "minishell.h"
#include <stdlib.h>
#include <string.h>

void	builtin_unset(char **args)
{
	int	i;

	if (!args[1])
		return ;
	i = 1;
	while (args[i])
	{
		if (strchr(args[i], '='))
		{
			fprintf(stderr, "minishell: unset: `%s': not a valid identifier\n", args[i]);
		}
		else
		{
			unsetenv(args[i]);
		}
		i++;
	}
}