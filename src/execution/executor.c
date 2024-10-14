/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/13 18:15:38 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/10 16:17:07 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void			execute_external(t_cmd *cmd, t_shell *shell);
static pid_t	fork_and_execute(t_cmd *cmd, int *pfds, int *prev_prd,
					t_shell *shell);
static pid_t	execute_single_command(t_cmd *cmd, int *prev_prd,
					t_shell *shell);
static void		wait_for_children(t_shell *shell, pid_t last_pid);
void			execute_command(t_shell *shell);

void	execute_external(t_cmd *cmd, t_shell *shell)
{
	char	*path;

	if (!cmd->argv[0])
	{
		shell->last_exit_status = 0;
		exit(shell->last_exit_status);
	}
	path = find_executable(cmd->argv[0], shell);
	if (!path)
	{
		ft_putstr_fd(cmd->argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	check_file_status(path, shell);
	execve(path, cmd->argv, shell->env);
	perror(path);
	free(path);
	exit(126);
}

static pid_t	fork_and_execute(t_cmd *cmd, int *pfds, int *prev_prd,
		t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		child_proc(cmd, pfds, *prev_prd, shell);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		parent_proc(cmd, pfds, prev_prd);
	}
	return (pid);
}

static pid_t	execute_single_command(t_cmd *cmd, int *prev_prd,
		t_shell *shell)
{
	int		pipe_fds[2];
	pid_t	pid;

	pid = -1;
	if (is_builtin_parent(cmd->argv[0]) && cmd->pipe_out == -1
		&& cmd->pipe_in == -1)
	{
		execute_builtin(cmd, shell);
	}
	else
	{
		if (cmd->pipe_out == 1)
		{
			if (pipe(pipe_fds) == -1)
			{
				perror("minishell: pipe");
				exit(EXIT_FAILURE);
			}
		}
		pid = fork_and_execute(cmd, pipe_fds, prev_prd, shell);
	}
	return (pid);
}

static void	wait_for_children(t_shell *shell, pid_t last_pid)
{
	int		status;
	pid_t	pid;

	pid = waitpid(-1, &status, WUNTRACED);
	while (pid > 0)
	{
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
		pid = waitpid(-1, &status, WUNTRACED);
	}
	if (pid == -1 && errno != ECHILD)
		perror("waitpid");
}

void	execute_command(t_shell *shell)
{
	t_cmd	*cur_cmd;
	int		prev_prd;
	pid_t	last_pid;

	prev_prd = -1;
	last_pid = -1;
	cur_cmd = shell->commands;
	while (cur_cmd)
	{
		last_pid = execute_single_command(cur_cmd, &prev_prd, shell);
		cur_cmd = cur_cmd->next;
	}
	if (last_pid != -1)
		wait_for_children(shell, last_pid);
	cleanup_heredoc_files(shell->commands);
	setup_signals_shell();
}
