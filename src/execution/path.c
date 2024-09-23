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
	char		*path_env;
	char		**paths;
	char		*full_path;

	// if (access(command, X_OK) == 0)
	// 	return (ft_strdup(command));
	path_env = ft_get_env("PATH", shell);
	if (!path_env)
		return (ft_strdup(command));
	paths = ft_split(path_env, ':');
	if (!paths)
		return (ft_strdup(command));
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
	struct stat	statbuf;

	if (ft_strchr(command, '/'))
	{
		if (stat(command, &statbuf) == -1)
		{
			if (errno == ENOENT)
				shell->last_exit_status = 127;
			else
				shell->last_exit_status = 126;
			return (NULL);
		}
		if (S_ISDIR(statbuf.st_mode))
		{
			shell->last_exit_status = 126;
			errno = EISDIR; // is dir
			return (NULL);
		}
		if (access(command, X_OK) == -1)
		{
			shell->last_exit_status = 126;
			return (NULL);
		}
		return (ft_strdup(command));
	}
	// if (access(command, X_OK) == 0)
	// 	return (ft_strdup(command));
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
	shell->last_exit_status = 127;
	return (NULL);
}

int	check_permissions(char *command, t_shell *shell)
{
	struct stat	statbuf;

	if (ft_strchr(command, '/'))
	{
		if (stat(command, &statbuf) == -1)
		{
			shell->last_exit_status = 126;
			ft_putstr_fd(" Error opening file\n", 2);
			exit(shell->last_exit_status);
		}
		if (stat(command, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
		{
			shell->last_exit_status = 126;
			ft_putstr_fd(" Is a directory\n", 2);
			// errno = EISDIR; // is dir
			exit(shell->last_exit_status);
		}
		if (access(command, X_OK) == -1)
		{
			shell->last_exit_status = 126;
			ft_putstr_fd(" Permission denied\n", 2);
			exit(shell->last_exit_status);
		}
	}
	return (1);
}
