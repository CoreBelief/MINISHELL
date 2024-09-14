/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   memory.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: elleneklund <elleneklund@student.codam.      +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/30 15:13:08 by elleneklund   #+#    #+#                 */
/*   Updated: 2024/09/14 02:35:57 by rdl           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_H
# define MEMORY_H

# include "minishell.h"

void	free_command(t_command *cmd);
void	free_command_list(t_command **head);
void	ft_free_str_array(char **arr);


#endif