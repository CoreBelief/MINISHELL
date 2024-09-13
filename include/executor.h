
#ifndef EXECUTOR_H
# define EXECUTOR_H

#include "minishell.h"

char		*find_executable(char *command, t_shell *shell);

void		execute_external(t_command *cmd, t_shell *shell);

void		setup_pipes(t_command *cmd, int pipe_fds[2]);

void		handle_child_process(t_command *cmd, int pipe_fds[2], int prev_pipe_read, t_shell *shell);

void		handle_parent_process(t_command *cmd, int pipe_fds[2], int *prev_pipe_read);

void		execute_single_command(t_command *cmd, int *prev_pipe_read, t_shell *shell);

void		wait_for_children(t_shell *shell);

void		execute_command(t_shell *shell);

#endif