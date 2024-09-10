#include "builtins.h"
#include "minishell.h"
#include <string.h>
#include <stdio.h>
#include "structs.h"



int	is_builtin_parent(char *command)
{
	int	i;

	i = 0;
	while (builtin_commands[i].name)
	{
		if (ft_strcmp(command, builtin_commands[i].name) == 0 && builtin_commands[i].is_parent)
		{
			return 1;
		}
		i++;
	}
	return 0;
}

int is_builtin(char *command)
{
	int i;

	i = 0;
	while (builtin_commands[i].name)
	{
		if (ft_strcmp(command, builtin_commands[i].name) == 0)
			return 1;
		i++;
	}
	return 0;
}

void execute_builtin(t_command *cmd)
{
    if (ft_strcmp(cmd->argv[0], "echo") == 0)
        builtin_echo(cmd->argv);
    else if (ft_strcmp(cmd->argv[0], "cd") == 0)
        builtin_cd(cmd->argv);
    else if (ft_strcmp(cmd->argv[0], "pwd") == 0)
        builtin_pwd(cmd->argv);
    else if (ft_strcmp(cmd->argv[0], "export") == 0)
        builtin_export(cmd->argv);
    else if (ft_strcmp(cmd->argv[0], "unset") == 0)
        builtin_unset(cmd->argv);
    else if (ft_strcmp(cmd->argv[0], "env") == 0)
        builtin_env(cmd->argv);
    else if (ft_strcmp(cmd->argv[0], "exit") == 0 || ft_strcmp(cmd->argv[0], "bye") == 0)
        builtin_exit(cmd->argv);
}

void    execute_external(t_command *cmd)
{
	char	*path;

	path = find_executable(cmd->argv[0]);
	execve(path, cmd->argv, environ);
	perror("minishell: execve failed\n");
	exit(EXIT_FAILURE);
}



