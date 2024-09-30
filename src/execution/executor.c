/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/13 18:15:38 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/30 17:22:19 by eeklund       ########   odam.nl         */
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

void execute_external(t_cmd *cmd, t_shell *shell)
{
    char *path;

    if (!cmd->argv[0])
    {
        shell->last_exit_status = 0;
        exit(shell->last_exit_status);
    }
    path = find_executable(cmd->argv[0], shell);
    if (!path)
    {
        shell->last_exit_status = 127;
        ft_putstr_fd(cmd->argv[0], 2);
        ft_putstr_fd(": command not found\n", 2);
        exit(shell->last_exit_status);
    }
    check_file_status(path, shell);
    if (strcmp(cmd->argv[0], "bash") == 0)
    {
        ft_set_env("PS1", "\\u@\\h:\\w\\$ ", shell);
        char *new_argv[4];
        new_argv[0] = "bash";
        new_argv[1] = "-i";
        new_argv[2] = NULL;
        int tty_fd = open("/dev/tty", O_RDWR);
        if (tty_fd != -1)
        {
            dup2(tty_fd, STDIN_FILENO);
            close(tty_fd);
        }

        execve(path, new_argv, shell->env);
    }
    else
    {
        int dev_null = open("/dev/null", O_WRONLY);
        if (dev_null != -1)
        {
            dup2(dev_null, STDERR_FILENO);
            close(dev_null);
        }
        execve(path, cmd->argv, shell->env);
    }
    dup2(STDERR_FILENO, 2);
    if (errno == EPIPE)
    {
        shell->last_exit_status = 141;
        free(path);
        exit(shell->last_exit_status);
    }
    shell->last_exit_status = 126;
    perror(path);
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
    else if (pid == 0)
	    child_proc(cmd, pfds, *prev_prd, shell);
    else
    {
        signal(SIGINT, SIG_IGN);
        parent_proc(cmd, pfds, prev_prd);
    }
    return pid;
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
		child_proc(cmd, pfds, *prev_prd, shell);
	else
	{
		signal(SIGINT, SIG_IGN);
		parent_proc(cmd, pfds, prev_prd);
	}
	return (pid);
}

static pid_t	execute_single_command(t_cmd *cmd, int *prev_prd, t_shell *shell)
{
	int		pipe_fds[2];
	pid_t	pid;

	pid = -1;
	if (is_builtin_parent(cmd->argv[0]) && cmd->pipe_out == -1 \
	&& cmd->pipe_in == -1)
	{
		execute_builtin(cmd, shell);
	}
	else
	{
		setup_pipes(cmd, pipe_fds);
		pid = fork_and_execute(cmd, pipe_fds, prev_prd, shell);
	}
	return (pid);
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
				unlink(cur->redir[i].file);
			i++;
		}
		cur = cur->next;
	}
}

static void	wait_for_children(t_shell *shell, pid_t last_pid)
{
	int status;
	pid_t pid;

	while ((pid = waitpid(-1, &status, WUNTRACED)) > 0)
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
	}
	if (pid == -1 && errno != ECHILD)
		perror("waitpid");
}

void execute_command(t_shell *shell)
{
    t_cmd *cur_cmd;
    int prev_prd = -1;
    pid_t last_pid = -1;

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

