/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/10 17:17:14 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/14 14:15:19 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error(char *cmd, char *msg);
void	print_command_not_found(char *command);
void	print_exit_numeric_error(char *arg);
void	*handle_syn_errors(int stat, char *msg, t_shell *shell);

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
