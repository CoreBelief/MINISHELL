/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtins.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/11 17:48:56 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/29 20:01:47 by rdl           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_builtin_parent(char *command);
int		is_builtin(char *command);
void	execute_builtin(t_cmd *cmd, t_shell *shell);

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

void	execute_builtin(t_cmd *cmd, t_shell *shell)
{
	if (ft_strcmp(cmd->argv[0], "echo") == 0)
		builtin_echo(cmd->argv);
	else if (ft_strcmp(cmd->argv[0], "cd") == 0)
		builtin_cd(cmd->argv, shell);
	else if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		builtin_pwd(cmd->argv);
	else if (ft_strcmp(cmd->argv[0], "export") == 0)
		builtin_export(cmd->argv, shell);
	else if (ft_strcmp(cmd->argv[0], "unset") == 0)
		builtin_unset(cmd->argv, shell);
	else if (ft_strcmp(cmd->argv[0], "env") == 0)
		builtin_env(cmd->argv, shell);
	else if (ft_strcmp(cmd->argv[0], "exit") == 0 || ft_strcmp(cmd->argv[0], "bye") == 0)
		builtin_exit(cmd->argv);
}


// void execute_builtin(t_cmd *cmd, t_shell *shell)
// {
//     if (ft_strcmp(cmd->argv[0], "echo") == 0)
//     {
//         shell->last_exit_status = builtin_echo(cmd->argv);
//     }
//     else if (ft_strcmp(cmd->argv[0], "cd") == 0)
//     {
//         shell->last_exit_status = builtin_cd(cmd->argv, shell);
//     }
//     else if (ft_strcmp(cmd->argv[0], "pwd") == 0)
//     {
//         shell->last_exit_status = builtin_pwd(cmd->argv);
//     }
//     else if (ft_strcmp(cmd->argv[0], "export") == 0)
//     {
//         shell->last_exit_status = builtin_export(cmd->argv, shell);
//     }
//     else if (ft_strcmp(cmd->argv[0], "unset") == 0)
//     {
//         shell->last_exit_status = builtin_unset(cmd->argv, shell);
//     }
//     else if (ft_strcmp(cmd->argv[0], "exit") == 0)
//     {
//         builtin_exit(cmd->argv, shell);
//     }
//     else
//     {
//         shell->last_exit_status = 127; // Command not found (default error code)
//         ft_putstr_fd("Command not found: ", 2);
//         ft_putstr_fd(cmd->argv[0], 2);
//         ft_putstr_fd("\n", 2);
//     }
// }




