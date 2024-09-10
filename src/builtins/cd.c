#include "minishell.h"
#include "environ.h"
#include <unistd.h>

static char	*get_cd_path(char **args)
{
	if (!args[1] || ft_strcmp(args[1], "~") == 0)
		return (getenv("HOME"));
	else if (ft_strcmp(args[1], "-") == 0)
		return (getenv("OLDPWD"));
	return (args[1]);
}

static void	update_pwd(char *old_pwd)
{
	char	new_pwd[1024];

	if (getcwd(new_pwd, sizeof(new_pwd)) != NULL)
	{
		ft_set_env("OLDPWD", old_pwd);
		ft_set_env("PWD", new_pwd);
	}
	else
		perror("getcwd");
}

void	builtin_cd(char **args)
{
	char	*path;
	char	old_pwd[1024];

	if (!getcwd(old_pwd, sizeof(old_pwd)))
	{
		perror("getcwd");
		return ;
	}
	path = get_cd_path(args);
	if (!path)
		return ;
	if (chdir(path) != 0)
	{
		perror("cd");
		return ;
	}
	update_pwd(old_pwd);
	// printf("old_pwd: %s\n", old_pwd);
}