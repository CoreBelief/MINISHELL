/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/30 19:32:27 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/04 18:23:46 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "minishell.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include "structs.h"

void		builtin_export(char **args, t_shell *shell);
static int	print_sorted_env(t_shell *shell);
static int	is_valid_identifier(const char *str);
static int	handle_value_assignment(char *arg, char *equal_sign, t_shell *shell);
static int	process_identifier(char *arg, char *equal_sign, t_shell *shell);
static void	sort_env(char **sorted_env, int size);

// too much functions in one file
// split it into multiple files?

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

static int	handle_value_assignment(char *arg, char *equal_sign, t_shell *shell)
{
	char	*value;

	*equal_sign = '\0';
	value = equal_sign + 1;
	if ((value[0] == '"' && value[ft_strlen(value) - 1] == '"') || \
	(value[0] == '\'' && value[ft_strlen(value) - 1] == '\''))
	{
		value[ft_strlen(value) - 1] = '\0';
		value++;
	}
	if (!ft_set_env(arg, value, shell))
		return (0);
	*equal_sign = '=';
	return (1);
}

static int	process_identifier(char *arg, char *equal_sign, t_shell *shell)
{
	char	*identifier;

	if (equal_sign != NULL){
		identifier = ft_strndup(arg, equal_sign - arg);
		if (!identifier)
		{
			shell->last_exit_status = 1;
			return (0);
		}
	}
	else
		identifier = arg;
	if (!is_valid_identifier(identifier))
	{
		print_error(arg, ": not a valid identifier\n"); //should we have the command name here(export?)
		shell->last_exit_status = 1;
		if (equal_sign != NULL)
			free(identifier);
		return (0);
	}
	if (equal_sign != NULL){
		if (!handle_value_assignment(arg, equal_sign, shell))
		{
			free(identifier);
			shell->last_exit_status = 1;
			return (0);
		}
	}
	else
	{
		if (!ft_set_env(arg, "", shell))
		{
			shell->last_exit_status = 1;
			return (0);
		}
	}
	if (equal_sign != NULL)
		free(identifier);
	return (1);
}

void	builtin_export(char **args, t_shell *shell)
{
	int		i;
	char	*equal_sign;

	i = 1;
	if (!args[1])
	{
		if (!print_sorted_env(shell))
		{
			shell->last_exit_status = 1;
			return ;
		}
	}
	while (args[i])
	{
		equal_sign = ft_strchr(args[i], '=');
		if (!process_identifier(args[i], equal_sign, shell))
			return ;
		i++;
	}
	shell->last_exit_status = 0;
}

static void	sort_env(char **sorted_env, int size)
{
	int		i;
	int		j;
	char	*temp;

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

static int	print_sorted_env(t_shell *shell)
{
	char	**sorted_env;
	int		i;
	char	*e_sign;

	sorted_env = malloc(sizeof(char *) * (shell->env_size + 1));
	if (!sorted_env)
		return (0);
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
	return (1);
}
