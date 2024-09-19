#include "minishell.h"
#include "environ.h"
#include <unistd.h>

static char	*get_cd_path(char **args, t_shell *shell);
static void	update_pwd(char *old_pwd, t_shell *shell);
void		builtin_cd(char **args, t_shell *shell);

static char	*get_cd_path(char **args, t_shell *shell)
{
	if (!args[1] || ft_strcmp(args[1], "~") == 0)
		return (ft_get_env("HOME", shell));
	else if (ft_strcmp(args[1], "-") == 0)
		return (ft_get_env("OLDPWD", shell));
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
	int		arg_count = 0;

	// Count the number of arguments
	while (args[arg_count])
		arg_count++;
	if (arg_count > 2)
	{
		fprintf(stderr, "cd: too many arguments\n");
		return;
	}
	if (!getcwd(old_pwd, sizeof(old_pwd)))
	{
		perror("getcwd");
		return ;
	}
	path = get_cd_path(args, shell);
	if (!path)
		return ;
	if (chdir(path) != 0)
	{
		perror("cd");
		return ;
	}
	update_pwd(old_pwd, shell);
	// printf("old_pwd: %s\n", old_pwd);
}