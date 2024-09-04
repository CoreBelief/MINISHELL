/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/13 18:15:38 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/04 15:39:29 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"

void	print_sorted_env(void);

char *find_executable(char *command)
{
    char *path = getenv("PATH");
    char *path_copy = ft_strdup(path);
    char *dir = strtok(path_copy, ":");
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

void	handle_child_process(t_command *cmd, int pipe_fds[2], int prev_pipe_read)
{
	// this needs to be fixed! ex. cat < txt | cat -e or ls | cat -e > txt doesnt work. 
	// have to implent good system for how the input/output should flow
	if (cmd->redirect_count)
		setup_redirections(cmd);
	// printf("Current command: %s and cmd->output == %i\n", cmd->argv[0], cmd->output);
	// fflush(stdout);
	if (prev_pipe_read != -1 && cmd->output == -1)
	{
		// printf("cmd: %s, read from prev_pipe_read == %i instead of stdin\n", cmd->argv[0], prev_pipe_read);
		dup2(prev_pipe_read, STDIN_FILENO);
		close(prev_pipe_read);
	}
	if (cmd->pipe_out == 1 && cmd->output == -1)
	{
		// printf("piping from stdout to pipe_fds[1] == %i\n", pipe_fds[1]);
		dup2(pipe_fds[1], STDOUT_FILENO);
		close(pipe_fds[1]);
	}
	close(pipe_fds[0]);
	if (is_builtin(cmd->argv[0]))
	{
		// printf("execute builtin\n");
		execute_builtin(cmd);
		exit(EXIT_SUCCESS);
	}
	else
	{
		// printf("executing external\n");
		execute_external(cmd);
	}
}

void	handle_parent_process(t_command *cmd, int pipe_fds[2], int *prev_pipe_read)
{
	if (*prev_pipe_read != -1)
		close(*prev_pipe_read);
	if (cmd->pipe_out == 1 && cmd->output == -1)
	{
		// printf("cur_cmd: %s, setting prev_pipe_read from pipe_fds[0] == %i\n", cur_cmd->argv[0], pipe_fds[0]);
		close(pipe_fds[1]);
		*prev_pipe_read = pipe_fds[0];
	}
	else
		*prev_pipe_read = -1;
}

void	execute_single_command(t_command *cmd, int *prev_pipe_read)
{
	int			pipe_fds[2];
	pid_t		pid;

	if (is_builtin_parent(cmd->argv[0]))
		execute_builtin(cmd);
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
			handle_child_process(cmd, pipe_fds, *prev_pipe_read);
		else
			handle_parent_process(cmd, pipe_fds, prev_pipe_read);
	}
}

void	execute_command(t_command *cmd)
{
	t_command	*cur_cmd;
	int			prev_pipe_read;
	int			status;

	cur_cmd = cmd;
	prev_pipe_read = -1;
	while (cur_cmd)
	{
		execute_single_command(cur_cmd, &prev_pipe_read);
		cur_cmd = cur_cmd->next;
	}
	while (wait(&status) > 0)
		;
	update_exit_status(status);
}

// shoudlent pipe it if there are redirections, then they will redirict, echo "input" > txt > test | cat txt soesnt work for example, i still need to fix how the stream goes.
// void execute_command(t_command *cmd)
// {
// 	// char		*path;
// 	pid_t		pid;
// 	int			status;
// 	int			pipe_fds[2];
// 	t_command	*cur_cmd;
// 	int			prev_pipe_read;

// 	prev_pipe_read = -1;
// 	cur_cmd = cmd;
// 	while (cur_cmd)
// 	{
// 		// printf("cur cmd %s\n", cur_cmd->argv[0]);
// 		// printf("fd %i\n", prev_pipe_read);
// 		if (cur_cmd->pipe_out == PIPE_OUT)
// 		{
// 			if (pipe(pipe_fds) == -1)
// 			{
// 				perror("minishell: pipe");
// 				exit(EXIT_FAILURE);
// 			}
// 		}
// 		if (is_builtin_parent(cmd->argv[0]))
// 		{
// 			execute_builtin(cmd);
// 			//implent some sort of pipe thingyyy
// 		}
// 		else
// 		{
// 			pid = fork();
// 			if (pid == -1)
// 			{
// 				perror("minishell: fork");
// 				exit(EXIT_FAILURE);
// 			}
// 			else if (pid == 0)
// 			{
// 				//in child
// 				printf("created fork\n");
// 				if (cur_cmd->redirect_count)
// 					setup_redirections(cur_cmd);
// 				printf("Current command: %s and cur_cmd->output == %i\n", cur_cmd->argv[0], cur_cmd->output);
// 				fflush(stdout);
// 				if (prev_pipe_read != -1 && cur_cmd->output == -1)
// 				{
// 					printf("cmd: %s, read from prev_pipe_read == %i instead of stdin\n", cur_cmd->argv[0], prev_pipe_read);
// 					dup2(prev_pipe_read, STDIN_FILENO);
// 					close(prev_pipe_read);
// 				}
// 				if (cur_cmd->pipe_out == PIPE_OUT && cur_cmd->output == -1)
// 				{
// 					printf("piping from stdout to pipe_fds[1] == %i\n", pipe_fds[1]);
// 					dup2(pipe_fds[1], STDOUT_FILENO);
// 					close(pipe_fds[1]);
// 				}
// 				close(pipe_fds[0]);
// 				if (is_builtin(cmd->argv[0]))
// 				{
// 					printf("execute builtin\n");
// 					execute_builtin(cmd);
// 					exit(EXIT_SUCCESS);
// 				}
// 				else
// 					execute_external(cur_cmd);
// 			}
// 			else
// 			{
// 				//in parent!
// 				printf("cur_cmd: %s, cur_cmd->output == %i\n", cur_cmd->argv[0], cur_cmd->output);
// 				if (prev_pipe_read != -1)
// 					close(prev_pipe_read);
// 				if (cur_cmd->pipe_out == PIPE_OUT && cur_cmd->output == -1)
// 				{
// 					printf("cur_cmd: %s, setting prev_pipe_read from pipe_fds[0] == %i\n", cur_cmd->argv[0], pipe_fds[0]);
// 					close(pipe_fds[1]);
// 					prev_pipe_read = pipe_fds[0];
// 				}
// 				else
// 					prev_pipe_read = -1;
// 			}
// 			// waitpid(pid, &status, 0);
// 			// update_exit_status(status);
// 		}
// 		cur_cmd = cur_cmd->next;
// 	}
// 	while (wait(&status) > 0)
// 		;
// 	update_exit_status(status);
// 	// waitpid(pid, &status, 0);
// 	// update_exit_status(status);
// 	// free_command(cmd);
// }


// OLD WORKING EXECUTOR
// void execute_command(t_command *cmd)
// {
// 	char		*path;
// 	pid_t		pid;
// 	int			status;

// 	if (is_builtin_parent(cmd->argv[0]))
// 		execute_builtin(cmd);
// 	// if (is_builtin(cmd->argv[0])
// 	// 	execute_builtin(cmd);
// 	else
// 	{
// 		pid = fork();
// 		if (pid == -1)
// 		{
// 			perror("minishell: fork");
// 		}
// 		else if (pid == 0)
// 		{
// 			setup_redirections(cmd);
// 			// if (ft_strcmp(cmd->argv[0], "echo") == 0)
// 			// {
// 			// 	// printf("DEBUG: Executing 'echo' builtin\n");
// 			// 	builtin_echo(cmd->argv);
// 			// }
// 			if (is_builtin(cmd->argv[0]))
// 			{
// 				execute_builtin(cmd);
// 				exit(EXIT_SUCCESS);
// 			}
// 			else
// 			{
// 				path = find_executable(cmd->argv[0]);
// 				// printf("external command: %s\n", path);
// 				execve(path, cmd->argv, environ);
// 				perror("minishell: execve failed\n");
// 				exit(EXIT_FAILURE);
// 			}
// 		}
// 		else
// 		{
// 			waitpid(pid, &status, 0);
// 			update_exit_status(status);
// 		}
// 	}
// 	free_command(cmd);
// }
