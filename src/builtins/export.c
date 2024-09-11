#include "builtins.h"
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//this is a global variable that is defined in the main.c file, so its forbidden!!
// extern char **shell->env;

 void	print_sorted_env(t_shell *shell)
{
	char	**sorted_env;
	int		i;
	int		j;
	char	*temp;

	sorted_env = malloc(sizeof(char *) * (shell->env_size + 1));
	i = -1;
	while (++i < shell->env_size)
		sorted_env[i] = shell->env[i];
	sorted_env[i] = NULL;
	i = -1;
	while (++i < shell->env_size - 1)
	{
		j = i;
		while (++j < shell->env_size)
		{
			if (ft_strcmp(sorted_env[i], sorted_env[j]) > 0)
			{
				temp = sorted_env[i];
				sorted_env[i] = sorted_env[j];
				sorted_env[j] = temp;
			}
		}
	}
	i = -1;
	while (++i < shell->env_size)
		printf("declare -x %s\n", sorted_env[i]);
	free(sorted_env);
}

// void	builtin_export(char **args)
// {
// 	int		i;
// 	char	*equal_sign;

// 	if (!args[1])
// 	{
// 		print_sorted_env();
// 		return ;
// 	}
// 	i = 1;
// 	while (args[i])
// 	{
// 		equal_sign = ft_strchr(args[i], '=');
// 		if (equal_sign)
// 		{
// 			*equal_sign = '\0';
// 			ft_set_env(args[i], equal_sign + 1, 1);
// 			*equal_sign = '=';
// 		}
// 		else
// 			setenv(args[i], "", 1);
// 		i++;
// 	}
// }

void	builtin_export(char **args, t_shell *shell)
{
	int		i;
	char	*equal_sign;

	if (!args[1])
	{
		print_sorted_env(shell);
		return ;
	}
	i = 1;
	while (args[i])
	{
		equal_sign = ft_strchr(args[i], '=');
		if (equal_sign)
		{
			*equal_sign = '\0';
			ft_set_env(args[i], equal_sign + 1, shell);
			*equal_sign = '=';
		}
		else
			ft_set_env(args[i], "", shell);
		i++;
	}
}