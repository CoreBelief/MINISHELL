/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/13 18:15:38 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/14 13:29:28 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"

void	execute_external(t_cmd *cmd, t_shell *shell);

void	setup_pipes(t_cmd *cmd, int pipe_fds[2]);

void	parent_proc(t_cmd *cmd, int pfds[2], int *prev_prd);

// void		execute_single_command(t_cmd *cmd, int *prev_prd, t_shell *shell);

// void		wait_for_children(t_shell *shell);

void	execute_command(t_shell *shell);

void	ft_strcpy_replace(char *dst, char *src, char *old, char *new);

char	*replace_substr(char *str, char *old, char *new);

char	*expand_exit(char *arg, t_shell *shell);

void	cleanup_heredoc_files(t_cmd *cmd);

#endif

// #ifndef EXECUTOR_H
// # define EXECUTOR_H

// #include "minishell.h"

// char		*find_executable(char *command, t_shell *shell);

// void		execute_external(t_command *cmd, t_shell *shell);

// void		setup_pipes(t_command *cmd, int pipe_fds[2]);

// // void		handle_child_process(t_command *cmd, int pipe_fds[2],
// 			int prev_pipe_read, t_shell *shell);

// // void		handle_parent_process(t_command *cmd, int pipe_fds[2],
// 			int *prev_pipe_read);

// // void		execute_single_command(t_command *cmd, int *prev_pipe_read,
// 			t_shell *shell);

// void		wait_for_children(t_shell *shell);

// void		execute_command(t_shell *shell);

// #endif