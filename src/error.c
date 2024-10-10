/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/10 17:17:14 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/10 17:17:33 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// this seems incomplete... we can put more error handling inside here, so the functions wil be shorter
void	print_error(char *cmd, char *msg);
void	print_command_not_found(char *command);
void	print_exit_numeric_error(char *arg);

void	print_error(char *cmd, char *msg)
{
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(msg, 2);
}

void	print_command_not_found(char *command)
{
	print_error(command, ": command not found\n");
	exit(127);
}

void	print_exit_numeric_error(char *arg)
{
	print_error(arg, ": numeric argument required\n");
	exit(2);
}

void	*handle_syn_errors(int stat, char *msg, t_shell *shell)
{
	shell->last_exit_status = stat;
	ft_putstr_fd(msg, 2);
	return (NULL);
}
