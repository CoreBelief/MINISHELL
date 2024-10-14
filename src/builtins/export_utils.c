/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export_utils.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/14 14:27:43 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/14 14:42:45 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_concatenated_value(const char *existing_value,
				const char *new_value);
static int	handle_append_assignment(char *arg, char *plus_equal_sign,
				t_shell *shell);
static int	handle_value_assignment(char *arg, char *equal_sign,
				t_shell *shell);
static int	set_environment(char *arg, char *equal_sign, t_shell *shell);
int			process_identifier(char *arg, char *equal_sign, t_shell *shell);

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

static int	handle_append_assignment(char *arg,
	char *equal_sign, t_shell *shell)
{
	char	*identifier;
	char	*new_value;
	char	*existing_value;
	char	*concatenated_value;
	int		result;

	identifier = NULL;
	identifier = ft_strndup(arg, (equal_sign - arg) - 1);
	if (!identifier)
		return (0);
	new_value = equal_sign + 1;
	existing_value = ft_get_env(identifier, shell);
	concatenated_value = get_concatenated_value(existing_value, new_value);
	if (!concatenated_value)
	{
		free(identifier);
		return (0);
	}
	result = ft_set_env(identifier, concatenated_value, shell);
	free(concatenated_value);
	free(identifier);
	return (result);
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
