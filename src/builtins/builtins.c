#include "builtins.h"
#include "minishell.h"
#include <string.h>
#include <stdio.h>

// char *find_executable(char *command);

static const char *builtin_parent_commands[] = {
    "cd", "export", "unset", "exit", "bye", NULL
};

static const char *builtin_commands[] = {
    "cd", "pwd", "export", "echo", "unset", "env", "exit", "bye", NULL
};


int	is_builtin_parent(char *command)
{
	int	i;

	i = 0;
	while (builtin_parent_commands[i])
	{
		if (ft_strcmp(command, builtin_parent_commands[i]) == 0)
		{
			return 1;
		}
		i++;
	}
	return 0;
}

void    execute_external(t_command *cmd, t_shell *shell)
{
	char	*path;

	printf("entered execute external\n");
	path = find_executable(cmd->argv[0], shell);
	printf("path %s\n", path);
	fflush(stdout);
	// print_sorted_env(shell);
	execve(path, cmd->argv, shell->env);
	perror("minishell: execve failed\n");
	exit(EXIT_FAILURE);
}

int is_builtin(char *command)
{
	int i;

	i = 0;
	while (builtin_commands[i])
	{
		if (ft_strcmp(command, builtin_commands[i]) == 0)
			return (1);
		i++;
	}
	return 0;
}

void execute_builtin(t_command *cmd, t_shell *shell)
{
	if (ft_strcmp(cmd->argv[0], "echo") == 0)
		builtin_echo(cmd->argv);
	else if (ft_strcmp(cmd->argv[0], "cd") == 0)
		builtin_cd(cmd->argv);
	else if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		builtin_pwd(cmd->argv);
	else if (ft_strcmp(cmd->argv[0], "export") == 0)
		builtin_export(cmd->argv, shell);
	else if (ft_strcmp(cmd->argv[0], "unset") == 0)
		builtin_unset(cmd->argv, shell);
	else if (ft_strcmp(cmd->argv[0], "env") == 0)
		builtin_env(cmd->argv, shell->env);
	else if (ft_strcmp(cmd->argv[0], "exit") == 0)
		builtin_exit(cmd->argv);
	else if (ft_strcmp(cmd->argv[0], "bye") == 0)
		builtin_exit(cmd->argv);
}
