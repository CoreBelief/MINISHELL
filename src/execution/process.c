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
void	wait_for_children(t_shell *shell);


//in exec_single_cmd if (is_builtin_parent(cmd->argv[0]) && cmd->pipe_out == -1 && cmd->pipe_in == -1)

void	child_proc(t_cmd *cmd, int pfds[2], int prev_prd, t_shell *shell)
{
	setup_signals_child();
	if (cmd->redirect_count)
		setup_redirections(cmd);
	if (prev_prd != -1 && cmd->input == -1)
	{
		// printf("Child process PID %d duplicated previous pipe %d to STDIN\n", getpid(), prev_prd);
		// dup2(prev_prd, STDIN_FILENO);
		 if (dup2(prev_prd, STDIN_FILENO) == -1)
        {
            perror("dup2 failed for STDIN");
            exit(EXIT_FAILURE);
        }
		// printf("Child process PID %d closed previous pipe %d\n", getpid(), prev_prd);
		close(prev_prd);
	}
	if (cmd->pipe_out == 1 && cmd->output == -1)
	{	
		// printf("File descriptor to duplicate: %d\n", pfds[1]);  // For STDOUT redirection

		// printf("Child process PID %d before duplicated pipe %d to STDOUT\n", getpid(), pfds[1]);
		// dup2(pfds[1], STDOUT_FILENO);
		if (dup2(pfds[1], STDOUT_FILENO) == -1)
			{
    			perror("dup2 failed for STDOUT");
    			exit(EXIT_FAILURE);
			}
		// printf("Child process PID %d before closed pipe write end %d\n", getpid(), pfds[1]);
		close(pfds[1]);
		// printf("Child process PID %d after closed pipe write end %d\n", getpid(), pfds[1]);
	}
	close(pfds[0]);
	// printf("Child process PID %d closed pipe read end %d\n", getpid(), pfds[0]);
	if (is_builtin(cmd->argv[0]))
	{
		execute_builtin(cmd, shell);
		exit(shell->last_exit_status);
	}
	else
		execute_external(cmd, shell);
	exit(EXIT_FAILURE);
}

void	parent_proc(t_cmd *cmd, int pfds[2], int *prev_prd)
{
	if (*prev_prd != -1)
	{
		// printf("Parent process closed previous pipe read end %d\n", *prev_prd);
		close(*prev_prd);
	}
	if (cmd->pipe_out == 1 && cmd->output == -1)
	{
		close(pfds[1]);
		// printf("Parent process closed pipe write end %d\n", pfds[1]);
		*prev_prd = pfds[0];
	}
	else
		*prev_prd = -1;
}



void	execute_child_process(t_cmd *cmd, int *pipe_fds,
			int prev_pipe_read, t_shell *shell)
{
	char	*path;

	path = find_command_in_path(cmd->argv[0], shell);
	if (path == NULL)
	{
		print_command_not_found(cmd->argv[0]);
		exit(127);
	}
	child_proc(cmd, pipe_fds, prev_pipe_read, shell);
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