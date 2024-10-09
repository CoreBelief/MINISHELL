/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   path.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/30 19:21:40 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/09 20:11:52 by rdl           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char		*search_paths(char **paths, char *command);
char		*find_executable(char *command, t_shell *shell);
static void	handle_error(t_shell *shell, char *path, int status, char *message);
void		check_file_status(char *path, t_shell *shell);

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

static void	handle_error(t_shell *shell, char *path, int status, char *message)
{
	shell->last_exit_status = status;
	ft_putstr_fd(path, 2);
	ft_putstr_fd(message, 2);
	free(path);
	free_command_list(&shell->commands);
	exit(shell->last_exit_status);
}

void	check_file_status(char *path, t_shell *shell)
{
	struct stat	st;

	if (stat(path, &st) != 0)
		handle_error(shell, path, 127, ": No such file or directory\n");
	if (S_ISDIR(st.st_mode))
		handle_error(shell, path, 126, ": Is a directory\n");
	if (!(st.st_mode & S_IXUSR))
		handle_error(shell, path, 126, ": Permission denied\n");
}
