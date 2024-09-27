/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/13 18:15:38 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/27 17:58:50 by rdl           ########   odam.nl         */
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

static int is_directory(char *path)
{
    struct stat statbuf;
    if (stat(path, &statbuf) != 0) 
        return 0;  // Couldn't retrieve file info (may not exist or be inaccessible)
    return S_ISDIR(statbuf.st_mode);  // Return 1 if it's a directory
}


void	execute_external(t_cmd *cmd, t_shell *shell)
{
	char	*path;

	// Step 1: Find the executable path
	path = find_executable(cmd->argv[0], shell);

	// Step 2: Check if the path was found or if it is NULL
	if (!path)
	{
		// Check if the file exists but is not executable
		if (access(cmd->argv[0], F_OK) == -1)
		{
			print_error(cmd->argv[0], "No such file or directory\n");
			shell->last_exit_status = 127;  // Set status for "file not found"
		}
		else if (access(cmd->argv[0], X_OK) == -1)
		{
			print_error(cmd->argv[0], "Permission denied\n");
			shell->last_exit_status = 126;  // Set status for "permission denied"
		}
		exit(shell->last_exit_status);
	}

	// Step 3: Check if the resolved path is a directory
	if (is_directory(path))
	{
		shell->last_exit_status = 126;  // Set exit status for "Is a directory"
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		free(path);  // Clean up allocated memory
		exit(shell->last_exit_status);  // Exit with status 126 (not executable)
	}

	// Step 4: Execute the command if it's not a directory
	execve(path, cmd->argv, shell->env);
	perror("minishell: execve failed\n");

	// Step 5: Handle specific error codes for execve failure
	if (errno == ENOENT)
	{
		print_error(path, "No such file or directory\n");
		shell->last_exit_status = 127;
	}
	else if (errno == EACCES)
	{
		ft_putstr_fd(" ", 2);  // Add a space between the filename and the message
        ft_putstr_fd("Permission denied\n", 2);
        shell->last_exit_status = 126;
	}

	free(path);  // Free the allocated path
	exit(shell->last_exit_status);  // Exit with the correct status
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
    else if (pid == 0)
	    child_proc(cmd, pfds, *prev_prd, shell);
    else
    {
        signal(SIGINT, SIG_IGN);
        parent_proc(cmd, pfds, prev_prd);
    }
    return pid;
}

static pid_t execute_single_command(t_cmd *cmd, int *prev_prd, t_shell *shell)
{
    int pipe_fds[2];
    pid_t pid = -1;

    if (is_builtin_parent(cmd->argv[0]) && cmd->pipe_out == -1 && cmd->pipe_in == -1)
        execute_builtin(cmd, shell);
    else
    {
        setup_pipes(cmd, pipe_fds);
        pid = fork_and_execute(cmd, pipe_fds, prev_prd, shell);
    }
    return pid;
}

void	cleanup_heredoc_files(t_cmd *cmd)
{
	int	i;

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
        cleanup_heredoc_files(cur_cmd);
        cur_cmd = cur_cmd->next;
    }
    wait_for_children(shell, last_pid);  
    setup_signals_shell();
}

