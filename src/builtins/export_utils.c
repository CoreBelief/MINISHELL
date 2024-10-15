/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export_utils.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/14 14:27:43 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/15 21:22:10 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	add_to_export_list(char *var, t_shell *shell);
static int	add_or_update_export_list(char *var, t_shell *shell);
static int	is_in_export_list(char *var, t_shell *shell);
static int	handle_append_assignment(char *arg, char *plus_equal_sign,
				t_shell *shell);
static int	handle_value_assignment(char *arg, char *equal_sign,
				t_shell *shell);
static int	set_environment(char *arg, char *equal_sign, t_shell *shell);
int			process_identifier(char *arg, char *equal_sign, t_shell *shell);
static int	is_matching_export(char *export, char *name);
static int	update_export_list(t_shell *shell, int i, char *var, char *name);
static char	*create_concatenated_value(char *identifier, char *new_value,
				t_shell *shell);
static int	update_env_and_export(char *identifier, char *value,
				t_shell *shell);

static int	add_to_export_list(char *var, t_shell *shell)
{
	char	**new_list;
	int		i;
	char	*tmp_var;

	new_list = malloc(sizeof(char *) * (shell->export_size + 2));
	if (!new_list)
		return (0);
	i = -1;
	while (++i < shell->export_size)
		new_list[i] = shell->export_list[i];
	free(shell->export_list);
	tmp_var = ft_strdup(var);
	if (!tmp_var)
	{
		free(new_list);
		return (0);
	}
	new_list[i] = tmp_var;
	new_list[i + 1] = NULL;
	shell->export_list = new_list;
	shell->export_size++;
	return (1);
}

static int	add_or_update_export_list(char *var, t_shell *shell)
{
	int		i;
	char	*name;
	char	*equal_sign;

	equal_sign = ft_strchr(var, '=');
	if (equal_sign)
		name = ft_strndup(var, equal_sign - var);
	else
		name = ft_strdup(var);
	if (!name)
		return (0);
	i = -1;
	while (++i < shell->export_size)
	{
		if (is_matching_export(shell->export_list[i], name))
		{
			return (update_export_list(shell, i, var, name));
		}
	}
	free(name);
	return (add_to_export_list(var, shell));
}

static int	is_matching_export(char *export, char *name)
{
	return (ft_strncmp(export, name, ft_strlen(name)) == 0
		&& (export[ft_strlen(name)] == '=' || export[ft_strlen(name)] == '\0'));
}

static int	update_export_list(t_shell *shell, int i, char *var, char *name)
{
	free(shell->export_list[i]);
	shell->export_list[i] = ft_strdup(var);
	free(name);
	return (shell->export_list[i] != NULL);
}

static int	is_in_export_list(char *var, t_shell *shell)
{
	int		i;
	size_t	len;

	len = ft_strlen(var);
	i = -1;
	while (++i < shell->export_size)
	{
		if (ft_strncmp(shell->export_list[i], var, len) == 0
			&& (shell->export_list[i][len] == '='
			|| shell->export_list[i][len] == '\0'))
			return (1);
	}
	return (0);
}

static int	handle_append_assignment(char *arg, char *equal_sign,
		t_shell *shell)
{
	char	*identifier;
	char	*concatenated_value;
	int		result;

	identifier = ft_strndup(arg, (equal_sign - arg) - 1);
	if (!identifier)
		return (0);
	concatenated_value = create_concatenated_value(identifier, equal_sign + 1,
			shell);
	if (!concatenated_value)
	{
		free(identifier);
		return (0);
	}
	result = update_env_and_export(identifier, concatenated_value, shell);
	free(concatenated_value);
	free(identifier);
	return (result);
}

static char	*create_concatenated_value(char *identifier, char *new_value,
		t_shell *shell)
{
	char	*existing_value;
	char	*concatenated_value;

	existing_value = ft_get_env(identifier, shell);
	if (existing_value)
		concatenated_value = ft_strjoin(existing_value, new_value);
	else
		concatenated_value = ft_strdup(new_value);
	return (concatenated_value);
}

static int	update_env_and_export(char *identifier, char *value, t_shell *shell)
{
	int		result;
	char	*new_var;
	char	*final_var;

	result = ft_set_env(identifier, value, shell);
	if (result)
	{
		new_var = ft_strjoin(identifier, "=");
		final_var = ft_strjoin(new_var, value);
		free(new_var);
		result = add_or_update_export_list(final_var, shell);
		free(final_var);
	}
	return (result);
}

static int	handle_value_assignment(char *arg, char *equal_sign, t_shell *shell)
{
	char	*plus_sign;

	plus_sign = equal_sign - 1;
	if (plus_sign >= arg && *plus_sign == '+')
	{
		return (handle_append_assignment(arg, equal_sign, shell));
	}
	*equal_sign = '\0';
	if (!ft_set_env(arg, equal_sign + 1, shell))
		return (0);
	*equal_sign = '=';
	return (add_or_update_export_list(arg, shell));
}

static int	set_environment(char *arg, char *equal_sign, t_shell *shell)
{
	if (equal_sign != NULL)
	{
		return (handle_value_assignment(arg, equal_sign, shell));
	}
	else if (!is_in_export_list(arg, shell))
	{
		return (add_or_update_export_list(arg, shell));
	}
	return (1);
}

int	process_identifier(char *arg, char *equal_sign, t_shell *shell)
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
