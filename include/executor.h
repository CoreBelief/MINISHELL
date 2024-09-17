
#ifndef EXECUTOR_H
# define EXECUTOR_H

#include "minishell.h"

char		*find_executable(char *command, t_shell *shell);

void		execute_external(t_cmd *cmd, t_shell *shell);

void		setup_pipes(t_cmd *cmd, int pipe_fds[2]);

void		child_proc(t_cmd *cmd, int pfds[2], int prev_prd, t_shell *shell);

void		parent_proc(t_cmd *cmd, int pfds[2], int *prev_prd);

void		execute_single_command(t_cmd *cmd, int *prev_prd, t_shell *shell);

void		wait_for_children(t_shell *shell);

void		execute_command(t_shell *shell);

#endif