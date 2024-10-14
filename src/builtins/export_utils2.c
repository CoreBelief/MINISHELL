/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export_utils2.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/14 14:38:06 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/14 14:43:47 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_identifier(const char *str);
int	validate_identifier(const char *identifier, char *arg,
		t_shell *shell);
int	handle_equal_sign(char *arg, char *equal_sign, char **identifier);
int	handle_no_equal_sign(char *arg, char **identifier);

int	is_valid_identifier(const char *str)
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

int	validate_identifier(const char *identifier, char *arg,
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

int	handle_equal_sign(char *arg, char *equal_sign, char **identifier)
{
	*identifier = ft_strndup(arg, equal_sign - arg);
	return (*identifier != NULL);
}

int	handle_no_equal_sign(char *arg, char **identifier)
{
	*identifier = arg;
	return (1);
}
