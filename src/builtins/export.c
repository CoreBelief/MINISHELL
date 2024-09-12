#include "builtins.h"
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void		builtin_export(char **args, t_shell *shell);
static void	print_sorted_env(t_shell *shell);

static void	print_sorted_env(t_shell *shell)
static void	print_sorted_env(t_shell *shell)
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

void	builtin_export(char **args, t_shell *shell)
	{
	int	i;

	i = 1;
	if (!args[1])
	{
		print_sorted_env(shell);
		return ;
	}
	while (args[i])
	{
		char *arg = args[i];
		char *equal_sign = ft_strchr(arg, '=');
		if (equal_sign)
		{
			*equal_sign = '\0';
			char *value = equal_sign + 1;
			
			// Remove surrounding quotes if present
			if ((value[0] == '"' && value[ft_strlen(value)-1] == '"') ||
				(value[0] == '\'' && value[ft_strlen(value)-1] == '\''))
			{
				value[ft_strlen(value)-1] = '\0';
				value++;
			}
			
			ft_set_env(arg, value, shell);
			*equal_sign = '=';
		}
		else
		{
			ft_set_env(arg, "", shell);
		}
		i++;
	}
}