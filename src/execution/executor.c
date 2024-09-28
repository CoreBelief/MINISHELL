/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/13 18:15:38 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/28 12:25:34 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include "error.h"
#include "path.h"
#include "process.h"

void	execute_external(t_cmd *cmd, t_shell *shell);
void	setup_pipes(t_cmd *cmd, int pipe_fds[2]);
void	execute_command(t_shell *shell);

// void	execute_external(t_cmd *cmd, t_shell *shell)
// {
// 	char	*path;

// 	path = find_command_in_path(cmd->argv[0], shell);
// 	if (!path)
// 	{
// 		if (shell->last_exit_status == 127)
// 			print_command_not_found(cmd->argv[0]);
// 		else if (shell->last_exit_status == 126)
// 		{
// 			if (errno == EISDIR)
// 				ft_putstr_fd(" Is a directory\n", 2);
// 			else if (errno == EACCES)
// 				ft_putstr_fd(" Permission denied\n", 2);
// 			else
// 				ft_putstr_fd(" Error opening file\n", 2);
// 		}
// 		exit(shell->last_exit_status);
// 	}
// 	execve(path, cmd->argv, shell->env);
// 	perror("minishell: execve failed\n");
// 	if (errno == EACCES)
// 	{
// 		ft_putstr_fd(" Permission denied\n", 2);
// 		shell->last_exit_status = 126;
// 	}
// 	else if (errno == ENOENT)
// 	{
// 		ft_putstr_fd("No such file or directory\n", 2);
// 		shell->last_exit_status = 127;
// 	}
// 	free(path);
// 	exit(shell->last_exit_status);
// }

void	execute_external(t_cmd *cmd, t_shell *shell)
{
	char	*path;

	path = find_executable(cmd->argv[0], shell);
	if (!path)
	{
		if (!path)
		{
			shell->last_exit_status = 127;
			print_command_not_found(cmd->argv[0]);
			exit(shell->last_exit_status);
		}
	}
	execve(path, cmd->argv, shell->env);
	perror("minishell: execve failed\n");
	if (errno == ENOENT)
	{
		print_error(path, "No such file or directory\n");
		shell->last_exit_status = 127;
	}
	if (errno == EACCES)
	{
		ft_putstr_fd(" Permission denied\n", 2);
		shell->last_exit_status = 126;
	}
	free(path);
	exit(shell->last_exit_status);

}

void	setup_pipes(t_cmd *cmd, int pfds[2])
{
	if (cmd->pipe_out == 1)
	{
		if (pipe(pfds) == -1)
		{
			perror("minishell: pipe");
			exit(EXIT_FAILURE);
		}
	}
}

static pid_t	fork_and_execute(t_cmd *cmd, int *pfds, int *prev_prd, t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)  // Child process
		child_proc(cmd, pfds, *prev_prd, shell);
	else  // Parent process
	{
		signal(SIGINT, SIG_IGN);
		parent_proc(cmd, pfds, prev_prd);
	}
	return (pid);  // Return the PID of the forked process
}

static pid_t	execute_single_command(t_cmd *cmd, int *prev_prd, t_shell *shell)
{
	int		pipe_fds[2];
	pid_t	pid;

	pid = -1;  // Store the PID of the forked process
	if (is_builtin_parent(cmd->argv[0]) && cmd->pipe_out == -1 \
	&& cmd->pipe_in == -1)
	{
		execute_builtin(cmd, shell);
	}
	else
	{
		setup_pipes(cmd, pipe_fds);
		pid = fork_and_execute(cmd, pipe_fds, prev_prd, shell);  // Capture the PID of the process
	}
	return pid;  // Return the PID of the last process
}




void	cleanup_heredoc_files(t_cmd *cmd)
{
	int		i;
	t_cmd	*cur;

	i = 0;
	cur = cmd;
	while (cur)
	{
		while (i < cur->redirect_count)
		{
			if (cur->redir[i].type == TOKEN_HEREDOC)
			{
				unlink(cur->redir[i].file);
				// free(cur->redir[i].file);
			}
			i++;
		}
		cur = cur->next;
	}
}

static void	wait_for_children(t_shell *shell, pid_t last_pid)
{
	int status;
	pid_t pid;

	// Wait for all child processes to finish
	while ((pid = waitpid(-1, &status, WUNTRACED)) > 0)
	{
		// If this is the last command's PID, update shell->last_exit_status
		if (pid == last_pid)
		{
			if (WIFSIGNALED(status))
			{
				shell->last_exit_status = 128 + WTERMSIG(status);
				if (WTERMSIG(status) == SIGQUIT)
					ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
					else if (WTERMSIG(status) == SIGINT)
					ft_putstr_fd("\n", STDERR_FILENO);
			}
			else if (WIFEXITED(status))
				shell->last_exit_status = WEXITSTATUS(status);
		}
	}

	if (pid == -1 && errno != ECHILD)
		perror("waitpid");
}

void	execute_command(t_shell *shell)
{
	t_cmd *cur_cmd;
	int prev_prd = -1;
	pid_t last_pid = -1;  // Store the PID of the last command

	cur_cmd = shell->commands;
	while (cur_cmd)
	{
		// Execute each command and capture the PID of the last command
		last_pid = execute_single_command(cur_cmd, &prev_prd, shell);
		cur_cmd = cur_cmd->next;
	}

	// After all commands are executed, wait for all children
	wait_for_children(shell, last_pid); 
	cleanup_heredoc_files(shell->commands);
	setup_signals_shell();
	// Pass the last command's PID
}

