/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/13 18:15:38 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/24 21:15:37 by rdl           ########   odam.nl         */
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


void	execute_external(t_cmd *cmd, t_shell *shell)
{
	char	*path;

	path = find_executable(cmd->argv[0], shell);
	if (!path)
	{	
		// printf("Executable for %s not found, PID: %d\n", cmd->argv[0], getpid());
		print_command_not_found(cmd->argv[0]);
		shell->last_exit_status = 127;
		exit(EXIT_FAILURE);
	}
	// printf("Child process PID %d executing %s\n", getpid(), cmd->argv[0]);
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

static pid_t fork_and_execute(t_cmd *cmd, int *pfds, int *prev_prd, t_shell *shell)
{
    pid_t pid;

    pid = fork();
    if (pid == -1)
    {
        perror("minishell: fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)  // Child process
    {
        execute_child_process(cmd, pfds, *prev_prd, shell);
    }
    else  // Parent process
    {
        signal(SIGINT, SIG_IGN);
        parent_proc(cmd, pfds, prev_prd);
    }
    return pid;  // Return the PID of the forked process
}

static pid_t execute_single_command(t_cmd *cmd, int *prev_prd, t_shell *shell)
{
    int pipe_fds[2];
    pid_t pid = -1;  // Store the PID of the forked process

    if (is_builtin_parent(cmd->argv[0]) && cmd->pipe_out == -1 && cmd->pipe_in == -1)
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
static void wait_for_children(t_shell *shell, pid_t last_pid)
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
                // Command was terminated by a signal
                shell->last_exit_status = 128 + WTERMSIG(status);
                if (WTERMSIG(status) == SIGQUIT)
                    ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
                    else if (WTERMSIG(status) == SIGINT)
                    ft_putstr_fd("\n", STDERR_FILENO);
            }
            else if (WIFEXITED(status))
            {
                // Command exited normally
                shell->last_exit_status = WEXITSTATUS(status);
            }
        }
    }

    if (pid == -1 && errno != ECHILD)
        perror("waitpid");
}
void execute_command(t_shell *shell)
{
    t_cmd *cur_cmd;
    int prev_prd = -1;
    pid_t last_pid = -1;  // Store the PID of the last command

    cur_cmd = shell->commands;
    while (cur_cmd)
    {
        // Execute each command and capture the PID of the last command
        last_pid = execute_single_command(cur_cmd, &prev_prd, shell);
        cleanup_heredoc_files(cur_cmd);
        cur_cmd = cur_cmd->next;
    }

    // After all commands are executed, wait for all children
    wait_for_children(shell, last_pid);  
    setup_signals_shell();
    // Pass the last command's PID
}

