
// #ifndef EXECUTOR_H
// # define EXECUTOR_H

// #include "minishell.h"

// char		*find_executable(char *command, t_shell *shell);

// void		execute_external(t_command *cmd, t_shell *shell);

// void		setup_pipes(t_command *cmd, int pipe_fds[2]);

// void		handle_child_process(t_command *cmd, int pipe_fds[2], int prev_pipe_read, t_shell *shell);

// void		handle_parent_process(t_command *cmd, int pipe_fds[2], int *prev_pipe_read);

// void		execute_single_command(t_command *cmd, int *prev_pipe_read, t_shell *shell);

// void		wait_for_children(t_shell *shell);

// void		execute_command(t_shell *shell);

// #endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/13 18:15:38 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/13 21:18:22 by rdl           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"

void		execute_external(t_cmd *cmd, t_shell *shell);

void		setup_pipes(t_cmd *cmd, int pipe_fds[2]);

void	fork_and_execute(t_cmd *cmd, int *pipe_fds,
			int *prev_prd, t_shell *shell);

void		parent_proc(t_cmd *cmd, int pfds[2], int *prev_prd);

void		execute_single_command(t_cmd *cmd, int *prev_prd, t_shell *shell);

void		wait_for_children(t_shell *shell);

void	execute_command(t_shell *shell);

#endif
