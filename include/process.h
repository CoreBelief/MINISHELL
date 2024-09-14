#ifndef PROCESS_H
# define PROCESS_H

# include "minishell.h"


void	handle_child_process(t_command *cmd, int pipe_fds[2],
			int prev_pipe_read, t_shell *shell);


void	handle_parent_process(t_command *cmd, int pipe_fds[2],
			int *prev_pipe_read);



void	execute_child_process(t_command *cmd, int *pipe_fds,
			int prev_pipe_read, t_shell *shell);

void	wait_for_children(t_shell *shell);

#endif
