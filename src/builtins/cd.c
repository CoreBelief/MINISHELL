/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/26 17:29:22 by rdl           #+#    #+#                 */
/*   Updated: 2024/10/17 13:04:47 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "environ.h"
#include "minishell.h"
#include <unistd.h>

static char	*get_cd_path(char **args, t_shell *shell);
static void	update_pwd(char *old_pwd, t_shell *shell);
static void	*handle_getcwd_error(t_shell *shell);
void		*builtin_cd(char **args, t_shell *shell);

static char	*get_cd_path(char **args, t_shell *shell)
{
	char	*home_path;
	char	*oldpwd;

	if (!args[1] || ft_strcmp(args[1], "~") == 0)
	{
		home_path = ft_get_env("HOME", shell);
		if (!home_path)
			return (handle_syn_errors(1, "cd: HOME not set\n", shell));
		return (home_path);
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		oldpwd = ft_get_env("OLDPWD", shell);
		if (!oldpwd)
			return (handle_syn_errors(1, "cd: OLDPWD not set\n", shell));
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

static void	*handle_getcwd_error(t_shell *shell)
{
	handle_syn_errors(1, "Minishell: cd: in unknown territory :O, \
	resetting to root\n", shell);
	if (chdir("/") == 0)
		shell->last_exit_status = 0;
	else
		handle_syn_errors(1, "Minishell: cd: Failed to reset to root\n", shell);
	return (NULL);
}

void	*builtin_cd(char **args, t_shell *shell)
{
	char	*path;
	char	old_pwd[1024];
	int		arg_count;

	arg_count = 0;
	while (args[arg_count])
		arg_count++;
	if (arg_count > 2)
		return (handle_syn_errors(1, \
		"Minishell: cd" ": too many arguments\n", shell));
	if (!getcwd(old_pwd, sizeof(old_pwd)))
		return (handle_getcwd_error(shell));
	path = get_cd_path(args, shell);
	if (!path)
		return (NULL);
	if (chdir(path) != 0)
	{
		handle_syn_errors(1, "Minishell: cd: ", shell);
		print_error(path, ": No such file or directory\n");
		return (NULL);
	}
	update_pwd(old_pwd, shell);
	shell->last_exit_status = 0;
	return (NULL);
}
