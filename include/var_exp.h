/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   var_exp.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/23 13:36:31 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/14 17:26:20 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef VAR_EXP_H
# define VAR_EXP_H

# include "minishell.h"

/* var_exp */
// int		handle_exit_status(char **new_str, int *i, t_shell *shell);
int		handle_variable(char **new_str, char *str, int *i, t_shell *shell);
// int		handle_dollar(char **new_str, char *str, int *i, t_shell *shell);
// int		handle_non_dollar(char **new_str, char *str, int *i);
char	*variable_exp_double(char *str, t_shell *shell);

/* var_exp_ utils */
int		is_var_char(char c);
int		find_var_len(char *var);
char	*append_str(char *og, char *to_append);
char	*variable_exp(char *str, int *i, t_shell *shell);
int		until_dollar(char *str);

#endif