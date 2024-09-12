/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/13 18:15:38 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/12 17:04:42 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"

void	print_sorted_env(void);

char *find_executable(char *command, t_shell *shell)
{
    char *path = ft_get_env("PATH", shell);
    char *path_copy = ft_strdup(path);
    char *dir = strtok(path_copy, ":"); //needs fixa
    char *full_path = NULL;

    while (dir != NULL) {
        full_path = ft_strjoin3(dir, "/", command);
        if (access(full_path, X_OK) == 0) {
            free(path_copy);
            return full_path;
        }
        free(full_path);
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return ft_strdup(command);  // Return the command as-is if not found in PATH
}

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

void	handle_child_process(t_command *cmd, int pipe_fds[2], int prev_pipe_read, t_shell *shell)
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
		exit(EXIT_SUCCESS);
	}
	else
		execute_external(cmd, shell);
}

void	handle_parent_process(t_command *cmd, int pipe_fds[2], int *prev_pipe_read)
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

void	execute_single_command(t_command *cmd, int *prev_pipe_read, t_shell *shell)
{
	int			pipe_fds[2];
	pid_t		pid;

	if (is_builtin_parent(cmd->argv[0]))
		execute_builtin(cmd, shell);
	else
	{
		setup_pipes(cmd, pipe_fds);
		pid = fork();
		if (pid == -1)
		{
			perror("minishell: fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
			handle_child_process(cmd, pipe_fds, *prev_pipe_read, shell);
		else
			handle_parent_process(cmd, pipe_fds, prev_pipe_read);
	}
}

void wait_for_children(t_shell *shell)
{
	int		status;
	pid_t	last_pid;

	while ((last_pid = wait(&status)) > 0)
	{
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT)
				shell->last_exit_status = 130;
			else if (WTERMSIG(status) == SIGQUIT)
			{
				shell->last_exit_status = 131;
				write(STDERR_FILENO, "Quit\n", 6);
			}
			else if (WTERMSIG(status) == SIGTERM)
				shell->last_exit_status = 143;
		}
		else if (WIFEXITED(status))
		{
			shell->last_exit_status = WEXITSTATUS(status);
		}
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

	wait_for_children(shell);
}
