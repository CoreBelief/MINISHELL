/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/13 18:15:38 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/15 18:42:55 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"

/* executor */
void	execute_external(t_cmd *cmd, t_shell *shell);
void	execute_command(t_shell *shell);

#endif
