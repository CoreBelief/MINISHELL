#include "minishell.h"
#include <errno.h>
#include "error.h"
#include "path.h"

void	child_proc(t_cmd *cmd, int pipe_fds[2],
			int prev_pipe_read, t_shell *shell);
void	parent_proc(t_cmd *cmd, int pipe_fds[2],
			int *prev_pipe_read);
void	execute_child_process(t_cmd *cmd, int *pipe_fds,
			int prev_pipe_read, t_shell *shell);


void handle_input_redirection(int prev_prd, t_cmd *cmd)
{
	if (prev_prd != -1 && cmd->input == -1)
	{
		if (dup2(prev_prd, STDIN_FILENO) == -1)
		{
			perror("dup2 failed for STDIN");
			exit(EXIT_FAILURE);
		}
		close(prev_prd);
	}
}

void handle_output_redirection(int pfds[2], t_cmd *cmd)
{
	if (cmd->pipe_out == 1 && cmd->output == -1)
	{
		if (dup2(pfds[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 failed for STDOUT");
			exit(EXIT_FAILURE);
		}
		close(pfds[1]);
	}
}

void close_pipe_ends(int pfds[2])
{
	close(pfds[0]);
	close(pfds[1]);
}

void	child_proc(t_cmd *cmd, int pfds[2], int prev_prd, t_shell *shell)
{
	setup_signals_child();
	signal(SIGPIPE, SIG_IGN);
	
	if (cmd->redirect_count)
		setup_redirections(cmd);

	handle_input_redirection(prev_prd, cmd);

	handle_output_redirection(pfds, cmd);

	close_pipe_ends(pfds);

	if (is_builtin(cmd->argv[0]))
	{
		execute_builtin(cmd, shell);
		exit(shell->last_exit_status);
	}
	else
	{
		execute_external(cmd, shell);
	}
	exit(EXIT_FAILURE);
}

void	parent_proc(t_cmd *cmd, int pfds[2], int *prev_prd)
{
	if (*prev_prd != -1)
		close(*prev_prd);
	if (cmd->pipe_out == 1 && cmd->output == -1)
	{
		close(pfds[1]);
		*prev_prd = pfds[0]; 
	}
	else
	{
		*prev_prd = -1;
		if (pfds[0] != -1)
			close(pfds[0]);
		if (pfds[1] != -1)
			close(pfds[1]);
	}
}

void	wait_for_children(t_shell *shell)
{
	int		status;
	pid_t	last_pid;

	while ((last_pid = waitpid(-1, &status, WUNTRACED)) > 0)
	{
		if (WIFSIGNALED(status))
		{
			shell->last_exit_status = 128 + WTERMSIG(status);
			if (WTERMSIG(status) == SIGQUIT)
				ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
		}
		else if (WIFEXITED(status))
			shell->last_exit_status = WEXITSTATUS(status);
	}
	if (last_pid == -1 && errno != ECHILD)
		perror("waitpid");
}
