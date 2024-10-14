/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   memory.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/30 15:13:08 by elleneklund   #+#    #+#                 */
/*   Updated: 2024/10/14 13:38:08 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_H
# define MEMORY_H

# include "minishell.h"

void	free_cmd_args(t_cmd *cmd);;
void	free_command(t_cmd *cmd);
void	free_command_list(t_cmd **head);
void	ft_free_str_array(char **arr);
void	free_tokens(t_token **head);

void	free_shell(t_shell *shell);

#endif