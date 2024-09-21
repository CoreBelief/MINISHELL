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
