/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/13 18:15:38 by eeklund       #+#    #+#                 */
/*   Updated: 2024/08/23 15:54:32 by rdl           ########   odam.nl         */
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

void execute_command(char *command)
{
	t_token		*tokens;
	t_command	*cmd;
	char		*path;
	pid_t		pid;
	int			status;

	printf("command: %s\n", command);
	tokens = tokenizer(command);
	if (!tokens)
		return ;

	cmd = parse_command_from_tokens(tokens);
	if (!cmd)
	{
		free_tokens(&tokens);
		return ;
	}
	// print_cmd_list(cmd);
	if (is_parent_command(cmd->argv[0]))
		execute_parent(cmd);
	// if (is_builtin(cmd->argv[0])
	// 	execute_builtin(cmd);
	else
	{
		pid = fork();
		if (pid == -1)
		{
			perror("minishell: fork");
		}
		else if (pid == 0)
		{
			setup_redirections(cmd);
			// if (ft_strcmp(cmd->argv[0], "echo") == 0)
			// {
			// 	// printf("DEBUG: Executing 'echo' builtin\n");
			// 	builtin_echo(cmd->argv);
			// }
			path = find_executable(cmd->argv[0]);
			printf("external command: %s\n", path);
			execve(path, cmd->argv, environ);
			perror("minishell: execve failed\n");
			exit(EXIT_FAILURE);
		}
		else
		{
			waitpid(pid, &status, 0);
			update_exit_status(status);
		}
	}
	free_command(cmd);
	free_tokens(&tokens);
}
