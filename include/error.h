/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/17 13:42:15 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/15 18:35:43 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# include "minishell.h" 

void	print_error(char *cmd, char *msg);

void	print_command_not_found(char *command);

void	print_exit_numeric_error(char *arg);

void	*handle_syn_errors(int stat, char *msg, t_shell *shell);

#endif
