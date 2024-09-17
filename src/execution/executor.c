/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/13 18:15:38 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/17 17:57:31 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
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


void	execute_external(t_cmd *cmd, t_shell *shell)
{
	char	*path;

	path = find_executable(cmd->argv[0], shell);
	if (!path)
	{
		print_command_not_found(cmd->argv[0]);
		shell->last_exit_status = 127;
		exit(EXIT_FAILURE);
	}
	execve(path, cmd->argv, shell->env);
	perror("minishell: execve failed\n");
	exit(EXIT_FAILURE);
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

void	execute_single_command(t_cmd *cmd, int *prev_prd, t_shell *shell)
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


void cleanup_heredoc_files(t_cmd *cmd)
{
	int i;

	i = 0;
	while (i < cmd->redirect_count)
	{
		if (cmd->redir[i].type == TOKEN_HEREDOC)
		{
			unlink(cmd->redir[i].file);
			// free(cmd->redir[i].file);
		}
		i++;
	}
}

void	execute_command(t_shell *shell)
{
	t_cmd	*cur_cmd;
	int			prev_prd;

	cur_cmd = shell->commands;
	prev_prd = -1;
	while (cur_cmd)
	{
		execute_single_command(cur_cmd, &prev_prd, shell);
		cleanup_heredoc_files(cur_cmd);
		cur_cmd = cur_cmd->next;
	}
}
