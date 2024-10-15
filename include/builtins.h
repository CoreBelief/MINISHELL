/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtins.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/10 17:18:28 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/15 19:20:14 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"
// # include "structs.h"

// Builtins.c
const t_builtin	*get_builtin_commands(void);
int				is_builtin_parent(char *command);
int				is_builtin(char *command);
void			execute_builtin(t_cmd *cmd, t_shell *shell);

// commands
void			*builtin_cd(char **args, t_shell *shell);

void			builtin_echo(char **args);

void			builtin_env(char **args, t_shell *shell);

void			builtin_exit(char **args);

void			builtin_export(char **args, t_shell *shell);

void			builtin_pwd(char **args, t_shell *shell);

void			builtin_unset(char **args, t_shell *shell);

// export utils
int				is_valid_identifier(const char *str);
int				validate_identifier(const char *identifier, char *arg,
					t_shell *shell);
int				handle_equal_sign(char *arg, char *equal_sign,
					char **identifier);
int				handle_no_equal_sign(char *arg, char **identifier);
int				process_identifier(char *arg, char *equal_sign, t_shell *shell);

#endif