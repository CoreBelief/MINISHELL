/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export_utils.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/14 14:27:43 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/15 20:37:22 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	add_to_export_list(char *var, t_shell *shell);
static int	is_in_export_list(char *var, t_shell *shell);
static int	set_env_with_equal(char *arg, char *equal_sign, t_shell *shell);
static int	set_environment(char *arg, char *equal_sign, t_shell *shell);
int			process_identifier(char *arg, char *equal_sign, t_shell *shell);

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
		return (0);
	new_list[i] = tmp_var;
	new_list[i + 1] = NULL;
	shell->export_list = new_list;
	shell->export_size++;
	return (1);
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

static int	set_env_with_equal(char *arg, char *equal_sign, t_shell *shell)
{
	printf("arg %s\n", arg);
	*equal_sign = '\0';
	if (!ft_set_env(arg, equal_sign + 1, shell))
		return (0);
	*equal_sign = '=';
	if (!is_in_export_list(arg, shell))
		return (add_to_export_list(arg, shell));
	return (1);
}

static int	set_environment(char *arg, char *equal_sign, t_shell *shell)
{
	if (equal_sign != NULL)
		return (set_env_with_equal(arg, equal_sign, shell));
	else if (!is_in_export_list(arg, shell))
		return (add_to_export_list(arg, shell));
	return (1);
}

int	process_identifier(char *arg, char *equal_sign, t_shell *shell)
{
	char	*identifier;
	int		result;

	if (equal_sign != NULL)
	{
		// printf("arg in process %s\n", arg);
		if (!handle_equal_sign(arg, equal_sign, &identifier))
		{
			shell->last_exit_status = 1;
			return (0);
		}
	}
	else
	{
		handle_no_equal_sign(arg, &identifier);
	}
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
