#include "minishell.h"

char	*search_paths(char **paths, char *command);
char	*find_executable(char *command, t_shell *shell);
char	*find_command_in_path(char *command, t_shell *shell);


char	*search_paths(char **paths, char *command)
{
	int		i;
	char	*full_path;

	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin3(paths[i], "/", command);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*find_executable(char *command, t_shell *shell)
{
	char	*path_env;
	char	**paths;
	char	*full_path;

	// if (access(command, X_OK) == 0)
	// 	return (ft_strdup(command)); // this is making minishellception work but gives other issues
	path_env = ft_get_env("PATH", shell);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	full_path = search_paths(paths, command);
	ft_free_str_array(paths);
	if (full_path)
		return (full_path);
	return (NULL);
}

char	*find_command_in_path(char *command, t_shell *shell)
{
	char	*path_env;
	char	**paths;
	char	*full_path;

	if (access(command, X_OK) == 0)
		return (ft_strdup(command));
	path_env = ft_get_env("PATH", shell);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	full_path = search_paths(paths, command);
	ft_free_str_array(paths);
	return (full_path);
}