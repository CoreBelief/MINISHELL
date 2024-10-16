/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export_utils3.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/15 21:51:13 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/15 22:05:53 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*create_concatenated_value(char *identifier, char *new_value,
				t_shell *shell);
static int	handle_append_assignment(char *arg, char *equal_sign,
				t_shell *shell);
static int	update_env_and_export(char *identifier, char *value,
				t_shell *shell);
static int	handle_value_assignment(char *arg, char *equal_sign,
				t_shell *shell);
int			set_environment(char *arg, char *equal_sign, t_shell *shell);

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

static int	update_env_and_export(char *identifier, char *value, t_shell *shell)
{
	int		result;
	char	*new_var;
	char	*final_var;

	result = ft_set_env(identifier, value, shell);
	if (result)
	{
		new_var = ft_strjoin(identifier, "=");
		if (!new_var)
			return (0);
		final_var = ft_strjoin(new_var, value);
		free(new_var);
		if (!final_var)
			return (0);
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

int	set_environment(char *arg, char *equal_sign, t_shell *shell)
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
