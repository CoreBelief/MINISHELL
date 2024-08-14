#include "builtins.h"
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;

 void	print_sorted_env(void)
{
	char	**sorted_env;
	int		env_size;
	int		i;
	int		j;
	char	*temp;

	env_size = 0;
	while (environ[env_size])
		env_size++;
	sorted_env = malloc(sizeof(char *) * (env_size + 1));
	i = -1;
	while (++i < env_size)
		sorted_env[i] = environ[i];
	sorted_env[i] = NULL;
	i = -1;
	while (++i < env_size - 1)
	{
		j = i;
		while (++j < env_size)
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
	while (++i < env_size)
		printf("declare -x %s\n", sorted_env[i]);
	free(sorted_env);
}

void	builtin_export(char **args)
{
	int		i;
	char	*equal_sign;

	if (!args[1])
	{
		print_sorted_env();
		return ;
	}
	i = 1;
	while (args[i])
	{
		equal_sign = ft_strchr(args[i], '=');
		if (equal_sign)
		{
			*equal_sign = '\0';
			setenv(args[i], equal_sign + 1, 1);
			*equal_sign = '=';
		}
		else
			setenv(args[i], "", 1);
		i++;
	}
}