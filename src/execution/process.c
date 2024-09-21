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


//in exec_single_cmd if (is_builtin_parent(cmd->argv[0]) && cmd->pipe_out == -1 && cmd->pipe_in == -1)

// void	child_proc(t_cmd *cmd, int pfds[2], int prev_prd, t_shell *shell)
// {
// 	setup_signals_child();
// 	if (cmd->redirect_count)
// 		setup_redirections(cmd);
// 	if (prev_prd != -1 && cmd->input == -1)
// 	{
// 		if (dup2(prev_prd, STDIN_FILENO) == -1)
// 		{
// 			perror("dup2 failed for STDIN");
// 			exit(EXIT_FAILURE);
// 		}
// 		close(prev_prd);
// 	}
// 	if (cmd->pipe_out == 1 && cmd->output == -1)
// 	{	
// 		if (dup2(pfds[1], STDOUT_FILENO) == -1)
// 		{
// 			perror("dup2 failed for STDOUT");
// 			exit(EXIT_FAILURE);
// 		}
// 		close(pfds[1]);
// 	}
// 	// Close both ends of the pipe in the child process
// 	close(pfds[0]);
// 	close(pfds[1]);
// 	if (is_builtin(cmd->argv[0]))
// 	{
// 		// printf("Executing builtin %s\n", cmd->argv[0]);
// 		execute_builtin(cmd, shell);
// 		exit(shell->last_exit_status);
// 	}
// 	else
// 		execute_external(cmd, shell);
// 	exit(EXIT_FAILURE);
// }

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
	signal(SIGPIPE, SIG_IGN); //sneaky peaky fix?
	
	// Setup redirections if needed
	if (cmd->redirect_count)
		setup_redirections(cmd);

	// Handle input redirection (dup2 and close)
	handle_input_redirection(prev_prd, cmd);

	// Handle output redirection (dup2 and close)
	handle_output_redirection(pfds, cmd);

	// Close pipe ends in the child process
	close_pipe_ends(pfds);

	// Execute either builtin or external command
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


// void	parent_proc(t_cmd *cmd, int pfds[2], int *prev_prd)
// {
// 	if (*prev_prd != -1)
// 	{
// 		close(*prev_prd);
// 	}
// 	if (cmd->pipe_out == 1 && cmd->output == -1)
// 	{
// 		close(pfds[1]);
// 		*prev_prd = pfds[0];
// 	}
// 	else
// 	{
// 		*prev_prd = -1;
// 		// Close both ends of the pipe if it's the last command
// 		close(pfds[0]);
// 		close(pfds[1]);
// 	}
// }

void	parent_proc(t_cmd *cmd, int pfds[2], int *prev_prd)
{
	// Close the previous pipe's read end if it exists
	if (*prev_prd != -1)
	{
		close(*prev_prd);
	}

	// If the current command is piping out, handle the pipe
	if (cmd->pipe_out == 1 && cmd->output == -1)
	{
		// Close the write end of the current pipe, and save the read end for the next command
		close(pfds[1]);
		*prev_prd = pfds[0];  // Save the read end for the next command
	}
	else
	{
		*prev_prd = -1;  // No more pipes, reset prev_prd
		
		// Close both ends of the pipe if it's the last command in the pipeline
		if (pfds[0] != -1)
			close(pfds[0]);  // Ensure the read end is closed
		if (pfds[1] != -1)
			close(pfds[1]);  // Ensure the write end is closed
	}
}




void	execute_child_process(t_cmd *cmd, int *pipe_fds,
			int prev_pipe_read, t_shell *shell)
{
	// if this is gone the function can be removed


	// char	*path;

	// path = find_command_in_path(cmd->argv[0], shell); // export VAR=hej | smth doesn't work
	// if (path == NULL)
	// {
	// 	print_command_not_found(cmd->argv[0]);
	// 	exit(127);
	// }

	child_proc(cmd, pipe_fds, prev_pipe_read, shell);
}


