/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   prompt_utils.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/10 18:41:01 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/10 18:45:13 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_username(void);
char	*get_current_dir(void);
char	*get_parent_dir(char *path);

char	*get_username(void)
{
	char	*username;
	char	*result;

	username = getenv("USER");
	if (username)
		result = ft_strdup(username);
	else
		result = ft_strdup("user");
	return (result);
}

char	*get_current_dir(void)
{
	char	cwd[MAX_PATH];
	char	*home;
	char	*relative_path;
	char	*result;

	if (!getcwd(cwd, sizeof(cwd)))
		return (ft_strdup("unknown"));
	home = getenv("HOME");
	if (home && ft_strncmp(cwd, home, ft_strlen(home)) == 0)
	{
		relative_path = cwd + ft_strlen(home);
		result = malloc(ft_strlen(relative_path) + 2);
		if (result)
		{
			result[0] = '~';
			ft_strcpy(result + 1, relative_path);
			return (result);
		}
	}
	return (ft_strdup(cwd));
}

char	*get_parent_dir(char *path)
{
	int		len;
	char	*parent_path;

	len = ft_strlen(path);
	while (len > 0 && path[len] != '/')
		len--;
	if (len == 0)
		return (ft_strdup("/"));
	parent_path = ft_substr(path, 0, len);
	return (parent_path);
}
