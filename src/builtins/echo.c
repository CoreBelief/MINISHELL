/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   echo.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/04 13:55:55 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/08 17:50:20 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_only_n_flag(char *arg);
void		builtin_echo(char **args);

static bool	is_only_n_flag(char *arg)
{
	int	i;

	if (arg[0] != '-')
		return (false);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

void	builtin_echo(char **args)
{
	int		i;
	bool	newline;

	i = 1;
	newline = true;
	if (!args)
		return ;
	while (args[i] && is_only_n_flag(args[i]))
	{
		newline = false;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
}
