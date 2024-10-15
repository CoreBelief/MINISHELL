/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   var_exp.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/23 13:36:31 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/15 19:07:02 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef VAR_EXP_H
# define VAR_EXP_H

# include "minishell.h"

/* var_exp */
char	*variable_exp_double(char *str, t_shell *shell);

/* var_exp_ utils */
int		handle_variable(char **new_str, char *str, int *i, t_shell *shell);
int		find_var_len(char *var);
char	*append_str(char *og, char *to_append);
int		until_dollar(char *str);

#endif