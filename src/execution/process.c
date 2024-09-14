#include "minishell.h"
#include <errno.h>
#include "error.h"
#include "path.h"

void	handle_child_process(t_command *cmd, int pipe_fds[2],
			int prev_pipe_read, t_shell *shell);
void	handle_parent_process(t_command *cmd, int pipe_fds[2],
			int *prev_pipe_read);
void	execute_child_process(t_command *cmd, int *pipe_fds,
			int prev_pipe_read, t_shell *shell);
void	wait_for_children(t_shell *shell);



void	handle_child_process(t_command *cmd, int pipe_fds[2],
			int prev_pipe_read, t_shell *shell)
{
	setup_signals_child();
	if (cmd->redirect_count)
		setup_redirections(cmd);
	if (prev_pipe_read != -1 && cmd->input == -1)
	{
		dup2(prev_pipe_read, STDIN_FILENO);
		close(prev_pipe_read);
	}
	if (cmd->pipe_out == 1 && cmd->output == -1)
	{
		dup2(pipe_fds[1], STDOUT_FILENO);
		close(pipe_fds[1]);
	}
	close(pipe_fds[0]);
	if (is_builtin(cmd->argv[0]))
	{
		execute_builtin(cmd, shell);
		exit(shell->last_exit_status);
	}
	else
		execute_external(cmd, shell);
	exit(EXIT_FAILURE);
}

void	handle_parent_process(t_command *cmd, int pipe_fds[2],
			int *prev_pipe_read)
{
	if (*prev_pipe_read != -1)
		close(*prev_pipe_read);
	if (cmd->pipe_out == 1 && cmd->output == -1)
	{
		close(pipe_fds[1]);
		*prev_pipe_read = pipe_fds[0];
	}
	else
		*prev_pipe_read = -1;
}


void	execute_child_process(t_command *cmd, int *pipe_fds,
			int prev_pipe_read, t_shell *shell)
{
	char	*path;

	path = find_command_in_path(cmd->argv[0], shell);
	if (path == NULL)
	{
		print_command_not_found(cmd->argv[0]);
		exit(127);
	}
	handle_child_process(cmd, pipe_fds, prev_pipe_read, shell);
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