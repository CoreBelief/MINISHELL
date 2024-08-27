/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/13 18:15:38 by eeklund       #+#    #+#                 */
/*   Updated: 2024/08/27 15:32:03 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"

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


void execute_command(t_command *cmd)
{
	char		*path;
	pid_t		pid;
	int			status;
	int			pipe_fds[2];
	t_command	*cur_cmd;
	int			prev_pipe_read;

	prev_pipe_read = -1;
	cur_cmd = cmd;
	while (cur_cmd)
	{
		printf("cur cmd %s\n", cur_cmd->argv[0]);
		printf("fd %i\n", prev_pipe_read);
		if (cur_cmd->pipe_out == PIPE_OUT)
		{
			if (pipe(pipe_fds) == -1)
			{
				perror("minishell: pipe");
				exit(EXIT_FAILURE);
			}
		}
		if (is_builtin_parent(cmd->argv[0]))
		{
			execute_builtin(cmd);
			//implent some sort of pipe thingyyy
		}
		else
		{
			pid = fork();
			if (pid == -1)
			{
				perror("minishell: fork");
			}
			else if (pid == 0)
			{
				//in child
				setup_redirections(cmd);
				if (prev_pipe_read != -1)
				{
					dup2(prev_pipe_read, STDIN_FILENO);
					close(prev_pipe_read);
				}
				if (cur_cmd->pipe_out)
				{
					close(pipe_fds[0]);
					dup2(pipe_fds[1], STDOUT_FILENO);
					close(pipe_fds[1]);
				}
				if (is_builtin(cmd->argv[0]))
				{
					execute_builtin(cmd);
					exit(EXIT_SUCCESS);
				}
				else
				{
					path = find_executable(cmd->argv[0]);
					printf("command: %s\n", path);
					execve(path, cmd->argv, environ);
					perror("minishell: execve failed\n");
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				//in parent!
				if (prev_pipe_read != -1)
					close(prev_pipe_read);
				if (cur_cmd->pipe_out == PIPE_OUT)
				{
					close(pipe_fds[1]);
					prev_pipe_read = pipe_fds[0];
				}
				else
					prev_pipe_read = -1;
			}
		}
		cur_cmd = cur_cmd->next;
	}
	while (wait(&status) > 0)
		;
	update_exit_status(status);
	free_command(cmd);
}


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
