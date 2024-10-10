/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtins.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/10 17:18:28 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/10 17:18:33 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

// Builtins.c
int		is_builtin_parent(char *command);
int		is_builtin(char *command);
void	execute_builtin(t_cmd *cmd, t_shell *shell);

// commands
void	builtin_cd(char **args, t_shell *shell);

void	builtin_echo(char **args);

void	builtin_env(char **args, t_shell *shell);

void	builtin_exit(char **args);

void	builtin_export(char **args, t_shell *shell);

void	builtin_pwd(char **args, t_shell *shell);

void	builtin_unset(char **args, t_shell *shell);

#endif