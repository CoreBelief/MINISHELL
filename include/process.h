/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   process.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/10 17:18:36 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/10 17:19:12 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROCESS_H
# define PROCESS_H

# include "minishell.h"

void	child_proc(t_cmd *cmd, int pipe_fds[2],
			int prev_pipe_read, t_shell *shell);

void	parent_proc(t_cmd *cmd, int pipe_fds[2],
			int *prev_pipe_read);

void	execute_child_process(t_cmd *cmd, int *pipe_fds,
			int prev_pipe_read, t_shell *shell);

#endif
