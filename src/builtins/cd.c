/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/26 17:29:22 by rdl           #+#    #+#                 */
/*   Updated: 2024/10/09 17:22:29 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "environ.h"
#include "minishell.h"
#include <unistd.h>

static char	*get_cd_path(char **args, t_shell *shell);
static void	update_pwd(char *old_pwd, t_shell *shell);
void		builtin_cd(char **args, t_shell *shell);

static char	*get_cd_path(char **args, t_shell *shell)
{
	char	*home_path;
	char	*oldpwd;

	if (!args[1] || ft_strcmp(args[1], "~") == 0)
	{
		home_path = ft_get_env("HOME", shell);
		if (!home_path)
		{
			print_error("cd", ": HOME not set\n");
			return (NULL);
		}
		return (home_path);
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		oldpwd = ft_get_env("OLDPWD", shell);
		if (!oldpwd)
		{
			print_error("cd", ": OLDPWD not set\n");
			return (NULL);
		}
		return (oldpwd);
	}
	return (args[1]);
}

static void	update_pwd(char *old_pwd, t_shell *shell)
{
	char	new_pwd[1024];

	if (getcwd(new_pwd, sizeof(new_pwd)) != NULL)
	{
		ft_set_env("OLDPWD", old_pwd, shell);
		ft_set_env("PWD", new_pwd, shell);
	}
	else
		perror("getcwd");
}

void	builtin_cd(char **args, t_shell *shell)
{
	char	*path;
	char	old_pwd[1024];
	int		arg_count;

	arg_count = 0;
	while (args[arg_count])
		arg_count++;
	if (arg_count > 2)
	{
		print_error("cd", ": too many arguments\n");
		shell->last_exit_status = 1;
		return ;
	}
	if (!getcwd(old_pwd, sizeof(old_pwd)))
	{
		perror("getcwd");
		shell->last_exit_status = 1;
		return ;
	}
	path = get_cd_path(args, shell);
	if (!path)
	{
		shell->last_exit_status = 1;
		return ;
	}
	if (chdir(path) != 0)
	{
		perror("cd");
		shell->last_exit_status = 1;
		return ;
	}
	update_pwd(old_pwd, shell);
	shell->last_exit_status = 0;
}
