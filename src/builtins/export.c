/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/30 19:32:27 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/10 15:31:28 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "minishell.h"

// #include <stdio.h>
// #include <stdlib.h>
// #include "structs.h"

static int	print_sorted_env(t_shell *shell);
static int	is_valid_identifier(const char *str);
static int	handle_value_assignment(char *arg, char *equal_sign,
				t_shell *shell);
static int	process_identifier(char *arg, char *equal_sign, t_shell *shell);
static void	sort_env(char **sorted_env, int size);
void		builtin_export(char **args, t_shell *shell);
static int	handle_append_assignment(char *arg, char *plus_equal_sign,
				t_shell *shell);
static char	*get_concatenated_value(const char *existing_value,
				const char *new_value);

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
	char	*plus_sign;

	plus_sign = equal_sign - 1;
	if (plus_sign >= arg && *plus_sign == '+')
	{
		return (handle_append_assignment(arg, equal_sign, shell));
	}
	*equal_sign = '\0';
	value = equal_sign + 1;
	if (!ft_set_env(arg, value, shell))
		return (0);
	*equal_sign = '=';
	return (1);
}

static int	handle_equal_sign(char *arg, char *equal_sign, char **identifier)
{
	*identifier = ft_strndup(arg, equal_sign - arg);
	return (*identifier != NULL);
}

static char	*get_concatenated_value(const char *existing_value,
		const char *new_value)
{
	char	*result;
	size_t	total_length;

	if (!existing_value)
		existing_value = "";
	total_length = ft_strlen(existing_value) + ft_strlen(new_value) + 1;
	result = malloc(total_length);
	if (!result)
		return (NULL);
	ft_strlcpy(result, existing_value, total_length);
	ft_strlcat(result, new_value, total_length);
	return (result);
}

static int	handle_append_assignment(char *arg, char *plus_equal_sign,
		t_shell *shell)
{
	char	*identifier;
	char	*new_value;
	char	*existing_value;
	char	*concatenated_value;
	int		result;

	*plus_equal_sign = '\0';
	identifier = arg;
	new_value = plus_equal_sign + 2; // Skip past '+='
	existing_value = ft_get_env(identifier, shell);
	concatenated_value = get_concatenated_value(existing_value, new_value);
	if (!concatenated_value)
	{
		*plus_equal_sign = '+'; // Restore the original string
		return (0);
	}
	result = ft_set_env(identifier, concatenated_value, shell);
	free(concatenated_value);
	*plus_equal_sign = '+'; // Restore the original string
	return (result);
}

static int	handle_no_equal_sign(char *arg, char **identifier)
{
	*identifier = arg;
	return (1);
}

static int	validate_identifier(const char *identifier, char *arg,
		t_shell *shell)
{
	if (!is_valid_identifier(identifier))
	{
		print_error(arg, ": not a valid identifier\n");
		shell->last_exit_status = 1;
		return (0);
	}
	return (1);
}

static int	set_environment(char *arg, char *equal_sign, t_shell *shell)
{
	if (equal_sign != NULL)
	{
		return (handle_value_assignment(arg, equal_sign, shell));
	}
	else
	{
		return (ft_set_env(arg, "", shell));
	}
}

static int	process_identifier(char *arg, char *equal_sign, t_shell *shell)
{
	char	*identifier;
	int		result;

	if (equal_sign != NULL)
	{
		if (!handle_equal_sign(arg, equal_sign, &identifier))
		{
			shell->last_exit_status = 1;
			return (0);
		}
	}
	else
		handle_no_equal_sign(arg, &identifier);
	if (!validate_identifier(identifier, arg, shell))
	{
		if (equal_sign != NULL)
			free(identifier);
		return (0);
	}
	result = set_environment(arg, equal_sign, shell);
	if (!result)
		shell->last_exit_status = 1;
	if (equal_sign != NULL)
		free(identifier);
	return (result);
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
		printf("declare -x %.*s\"%s\"\n", (int)(e_sign - sorted_env[i] + 1),
			sorted_env[i], e_sign + 1);
	}
	free(sorted_env);
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
