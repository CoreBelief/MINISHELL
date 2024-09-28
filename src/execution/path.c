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

// char	*find_executable(char *command, t_shell *shell)
// {
// 	char		*path_env;
// 	char		**paths;
// 	char		*full_path;

// 	// if (access(command, X_OK) == 0)
// 	// 	return (ft_strdup(command));
// 	path_env = ft_get_env("PATH", shell);
// 	// printf("path_env: %s\n", path_env);
// 	if (!path_env)
// 		return (ft_strdup(command));
// 	paths = ft_split(path_env, ':');
// 	// printf("paths: %s\n", paths[0]);
// 	if (!paths)
// 		return (ft_strdup(command));
// 	full_path = search_paths(paths, command);
// 	// printf("full_path: %s\n", full_path);
// 	ft_free_str_array(paths);
// 	if (full_path)
// 		return (full_path);
// 	return (NULL);
// }

char	*find_executable(char *command, t_shell *shell)
{
	char	*path_env;
	char	**paths;
	char	*full_path;

	if (!command || command[0] == '\0')
		return (NULL);
	if (command[0] == '/' || (command[0] == '.' && (command[1] == '/' || \
	(command[1] == '.' && command[2] == '/'))))
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

void	check_file_status(char *path, t_shell *shell)
{
	struct stat	st;

	if (stat(path, &st) != 0)
	{
		shell->last_exit_status = 127;
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		free(path);
		free_command_list(&shell->commands);
		exit(shell->last_exit_status);
	}
	if (S_ISDIR(st.st_mode))
	{
		shell->last_exit_status = 126;
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		free(path);
		free_command_list(&shell->commands);
		exit(shell->last_exit_status);
	}
	if (!(st.st_mode & S_IXUSR))
	{
		shell->last_exit_status = 126;
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		free(path);
		free_command_list(&shell->commands);
		exit(shell->last_exit_status);
	}
}
