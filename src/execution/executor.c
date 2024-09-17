/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/13 18:15:38 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/17 14:42:47 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
// #include "error.h"

char		*find_executable(char *command, t_shell *shell);
void		execute_external(t_cmd *cmd, t_shell *shell);
void		setup_pipes(t_cmd *cmd, int pfds[2]);
void		child_proc(t_cmd *cmd, int pfds[2], int prev_prd, t_shell *shell);
void		parent_proc(t_cmd *cmd, int pfds[2], int *prev_prd);
void		execute_single_command(t_cmd *cmd, int *prev_prd, t_shell *shell);
void		wait_for_children(t_shell *shell);
void		execute_command(t_shell *shell);

char *find_executable(char *command, t_shell *shell) //illegal function!!!
{
    char *path = ft_get_env("PATH", shell);
    char *path_copy = ft_strdup(path);
    char *dir = strtok(path_copy, ":"); //needs fixa
    char *full_path = NULL;

    while (dir != NULL) {
        full_path = ft_strjoin3(dir, "/", command);
        if (access(full_path, X_OK) == 0) {
            free(path_copy);
            return (full_path);
        }
        free(full_path);
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return (NULL);  // Return the command as-is if not found in PATH
}


char *find_command_in_path(char *command) //another illegal function!!!
{
    // Sample function to search command in PATH
    char *path_env = getenv("PATH");  // Get PATH environment variable
    char *path = strtok(path_env, ":");  // Split PATH by ':'
    
    while (path != NULL) {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, command);  // Concatenate path and command
        
        if (access(full_path, X_OK) == 0) {  // Check if command is executable
            return strdup(full_path);  // Return full path to the command
        }
        path = strtok(NULL, ":");
    }
    return NULL;  // Command not found
}


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

void	child_proc(t_cmd *cmd, int pfds[2], int prev_prd, t_shell *shell)
{
	setup_signals_child();
	if (cmd->redirect_count)
		setup_redirections(cmd);
	if (prev_prd != -1 && cmd->input == -1)
	{
		dup2(prev_prd, STDIN_FILENO);
		close(prev_prd);
	}
	if (cmd->pipe_out == 1 && cmd->output == -1)
	{
		dup2(pfds[1], STDOUT_FILENO);
		close(pfds[1]);
	}
	close(pfds[0]);
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
		close(*prev_prd);
	if (cmd->pipe_out == 1 && cmd->output == -1)
	{
		close(pfds[1]);
		*prev_prd = pfds[0];
	}
	else
		*prev_prd = -1;
}

void	execute_single_command(t_cmd *cmd, int *prev_prd, t_shell *shell)
{
	int			pfds[2];
	pid_t		pid;
	// char 		*path;

	if (is_builtin_parent(cmd->argv[0]) && cmd->pipe_out == -1 && cmd->pipe_in == -1)
		execute_builtin(cmd, shell);
	else
	{
		setup_pipes(cmd, pfds);
		pid = fork();
		if (pid == -1)
		{
			perror("minishell: fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{	
			// path = find_command_in_path(cmd->argv[0]);
			// if (path == NULL) 
			// {
            // print_cmd_not_found(cmd->argv[0]);
			// exit(127);   // Call this when a command is not found
        	// }
			child_proc(cmd, pfds, *prev_prd, shell);
		}
		else
		{
			signal(SIGINT, SIG_IGN);
			parent_proc(cmd, pfds, prev_prd);
			// Wait for child to complete
			wait_for_children(shell);
			// Restore SIGINT handler for parent shell after the child has finished
			setup_signals_shell();
		}
	}
}

// void	wait_for_children(t_shell *shell)
// {
// 	int		status;
// 	pid_t	last_pid;

// 	while ((last_pid = waitpid(-1, &status, WUNTRACED)) > 0)
// 	{
// 		if (WIFSIGNALED(status))
// 		{
// 			shell->last_exit_status = 128 + WTERMSIG(status);
// 			if (WTERMSIG(status) == SIGQUIT)
// 				ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
// 		}
// 		else if (WIFSTOPPED(status)) //this never happens??? since we block it in setup_signals_child
// 		{
// 			shell->last_exit_status = 128 + WSTOPSIG(status);
// 			printf("Stopped: %d\n", WSTOPSIG(status));
// 		}
// 		else if (WIFEXITED(status))
// 			shell->last_exit_status = WEXITSTATUS(status);
// 	}
// 	if (last_pid == -1 && errno != ECHILD)
// 		perror("waitpid");
// }
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
