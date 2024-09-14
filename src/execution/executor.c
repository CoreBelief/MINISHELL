/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/13 18:15:38 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/14 03:10:16 by rdl           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"
#include <errno.h>
#include "error.h"
#include "path.h"
#include "process.h"

void	execute_external(t_command *cmd, t_shell *shell);
void	setup_pipes(t_command *cmd, int pipe_fds[2]);
void	fork_and_execute(t_command *cmd, int *pipe_fds,
			int *prev_pipe_read, t_shell *shell);
void	execute_single_command(t_command *cmd, int *prev_pipe_read,
			t_shell *shell);
void	execute_command(t_shell *shell);


void	execute_external(t_command *cmd, t_shell *shell)
{
	char	*path;

	path = find_executable(cmd->argv[0], shell);
	execve(path, cmd->argv, shell->env);
	perror("minishell: execve failed\n");
	exit(EXIT_FAILURE);
}

void	setup_pipes(t_command *cmd, int pipe_fds[2])
{
	if (cmd->pipe_out == 1)
	{
		if (pipe(pipe_fds) == -1)
		{
			perror("minishell: pipe");
			exit(EXIT_FAILURE);
		}
	}
}


void	fork_and_execute(t_command *cmd, int *pipe_fds,
			int *prev_pipe_read, t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
		execute_child_process(cmd, pipe_fds, *prev_pipe_read, shell);
	else
	{
		signal(SIGINT, SIG_IGN);
		handle_parent_process(cmd, pipe_fds, prev_pipe_read);
		wait_for_children(shell);
		setup_signals_shell();
	}
}

void	execute_single_command(t_command *cmd, int *prev_pipe_read,
			t_shell *shell)
{
	int		pipe_fds[2];

	if (is_builtin_parent(cmd->argv[0]))
		execute_builtin(cmd, shell);
	else
	{
		setup_pipes(cmd, pipe_fds);
		fork_and_execute(cmd, pipe_fds, prev_pipe_read, shell);
	}
}



void	execute_command(t_shell *shell)
{
	t_command	*cur_cmd;
	int			prev_pipe_read;

	cur_cmd = shell->commands;
	prev_pipe_read = -1;
	while (cur_cmd)
	{
		execute_single_command(cur_cmd, &prev_pipe_read, shell);
		cur_cmd = cur_cmd->next;
	}
}
