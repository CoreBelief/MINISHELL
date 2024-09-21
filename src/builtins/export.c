
#include "builtins.h"
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include "structs.h"


// too much functions in one file
// split it into multiple files?

void	builtin_export(char **args, t_shell *shell);
static void	print_sorted_env(t_shell *shell);

static int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !str[0] || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void handle_value_assignment(char *arg, char *equal_sign, t_shell *shell)
{
	char *value;

	*equal_sign = '\0';
	value = equal_sign + 1;
	if ((value[0] == '"' && value[ft_strlen(value) - 1] == '"') ||
		(value[0] == '\'' && value[ft_strlen(value) - 1] == '\''))
	{
		value[ft_strlen(value) - 1] = '\0';
		value++;
	}
	ft_set_env(arg, value, shell);
	*equal_sign = '=';
}

static void process_identifier(char *arg, char *equal_sign, t_shell *shell)
{
	char *identifier;

	if (equal_sign != NULL)
		identifier = ft_strndup(arg, equal_sign - arg);
	else
		identifier = arg;
	
	if (!is_valid_identifier(identifier))
	{
		dprintf(2, "export: `%s`: not a valid identifier\n", arg);
		shell->last_exit_status = 1;
		if (equal_sign != NULL)
			free(identifier);
		return;
	}

	if (equal_sign != NULL)
		handle_value_assignment(arg, equal_sign, shell);
	else
		ft_set_env(arg, "", shell);

	if (equal_sign != NULL)
		free(identifier);
}


void	builtin_export(char **args, t_shell *shell)
{
	int		i;
	char	*equal_sign;

	i = 1;
	if (!args[1])
	{
		print_sorted_env(shell);
		return ;
	}
	while (args[i])
	{
		equal_sign = ft_strchr(args[i], '=');
		process_identifier(args[i], equal_sign, shell);
		i++;
	}
}

static void	sort_env(char **sorted_env, int size)
{
	int		i;
	int		j;
	char	*temp;
	char	*e_sign;

	i = -1;
	while (++i < size - 1)
	{
		j = i;
		while (++j < size)
		{
			if (ft_strcmp(sorted_env[i], sorted_env[j]) > 0)
			{
				temp = sorted_env[i];
				sorted_env[i] = sorted_env[j];
				sorted_env[j] = temp;
			}
		}
	}
}

static void	print_sorted_env(t_shell *shell)
{
	char	**sorted_env;
	int		i;

	sorted_env = malloc(sizeof(char *) * (shell->env_size + 1));
	if (!sorted_env)
		return ;
	i = -1;
	while (++i < shell->env_size)
		sorted_env[i] = shell->env[i];
	sorted_env[i] = NULL;
	sort_env(sorted_env, shell->env_size);
	i = -1;
	while (++i < shell->env_size)
	{
		e_sign = ft_strchr(sorted_env[i], '=');
		printf("declare -x %.*s\"%s\"\n", (int)(e_sign - sorted_env[i] + 1), \
		sorted_env[i], e_sign + 1);
	}
	free(sorted_env);
}
