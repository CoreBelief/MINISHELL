#include "builtins.h"
#include "minishell.h"
#include <stdlib.h>
#include <string.h>
void	builtin_unset(char **args, t_shell *shell);

void	builtin_unset(char **args, t_shell *shell)
{
	int	i;

	if (!args[1])
		return ;
	i = 1;
	while (args[i])
	{
		if (ft_strchr(args[i], '='))
		{
			fprintf(stderr, "minishell: unset: `%s': not a valid identifier\n", args[i]);
		}
		else
		{
			ft_unset_env(args[i], shell);
		}
		i++;
	}
}