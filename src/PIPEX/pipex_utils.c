/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: skoene <skoene@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/23 17:11:54 by skoene        #+#    #+#                 */
/*   Updated: 2024/07/26 15:11:34 by skoene        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error(void)
{
	perror("\033[31mError");
	exit(EXIT_FAILURE);
}

char	**get_paths(char **envp)
{
	int		i;
	char	**paths;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
	{
		ft_putstr_fd("Error: PATH not found in environment.\n", 2);
		return (NULL);
	}
	paths = ft_split(envp[i] + 5, ':');
	if (!paths)
	{
		ft_putstr_fd("Error: Memory allocation failed during path splitting.\n",
			2);
		return (NULL);
	}
	return (paths);
}

char	*construct_path(char *directory, char *cmd)
{
	char	*part_path;
	char	*full_path;

	part_path = ft_strjoin(directory, "/");
	if (!part_path)
	{
		ft_putstr_fd("Error: Memory alloc failed during path construc\n", 2);
		return (NULL);
	}
	full_path = ft_strjoin(part_path, cmd);
	free(part_path);
	if (!full_path)
	{
		ft_putstr_fd("Error: Memory alloc failed during full path constr\n", 2);
		return (NULL);
	}
	return (full_path);
}

char	*find_path(char *cmd, char **envp)
{
	char	**paths;
	char	*full_path;
	int		i;

	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	paths = get_paths(envp);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = construct_path(paths[i], cmd);
		if (!full_path)
			return (ft_split_free(paths), NULL);
		if (access(full_path, X_OK) == 0)
			return (ft_split_free(paths), full_path);
		free(full_path);
		i++;
	}
	return (ft_split_free(paths), NULL);
}

void	execute(char *argv, char **envp)
{
	char	**cmd;
	char	*path;

	cmd = ft_split(argv, ' ');
	if (!cmd || !cmd[0])
	{
		ft_putstr_fd("Error: Invalid or empty command.\n", 2);
		ft_split_free(cmd);
		exit(EXIT_FAILURE);
	}
	path = find_path(cmd[0], envp);
	if (!path)
	{
		ft_putstr_fd("Error: Command not found.\n", 2);
		ft_split_free(cmd);
		exit(127);
	}
	if (execve(path, cmd, envp) == -1)
	{
		ft_putstr_fd("Error: execve failed.\n", 2);
		free(path);
		ft_split_free(cmd);
		exit(EXIT_FAILURE);
	}
}
