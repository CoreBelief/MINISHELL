/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   var_exp.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/23 13:36:31 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/25 17:41:05 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef VAR_EXP_H
# define VAR_EXP_H

# include "minishell.h"

int		variable_exp_double(t_token *token, char *str, t_shell *shell);
int		until_dollar(char *str);
void	variable_exp_dollar(t_token *token, char *str, t_shell *shell);
int		is_var_char(char c);
int		find_var_len(char *var);

#endif