/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   prompt.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/10 18:39:26 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/10 18:41:54 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#define MAX_PATH 1024
#include "minishell.h"
#include <dirent.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

#define COLOR_GREEN "\001\033[1;32m\002"
#define COLOR_CYAN "\001\033[1;36m\002"
#define COLOR_YELLOW "\001\033[1;33m\002"
#define COLOR_RESET "\001\033[0m\002"

// too many functions in fil, maybe make separte git file?

static char	*append_to_prompt(char *prompt, char *str);
static int	check_git_in_dir(char *dir_path);
static int	is_git_repo(void);
char		*create_prompt(void);

static char	*append_to_prompt(char *prompt, char *str)
{
	char	*temp;

	temp = ft_strjoin(prompt, str);
	free(prompt);
	return (temp);
}

static int	check_git_in_dir(char *dir_path)
{
	DIR				*dir;
	struct dirent	*entry;

	dir = opendir(dir_path);
	if (!dir)
		return (0);
	while ((entry = readdir(dir)) != NULL)
	{
		if (ft_strcmp(entry->d_name, ".git") == 0)
		{
			closedir(dir);
			return (1);
		}
	}
	closedir(dir);
	return (0);
}

static int	is_git_repo(void)
{
	char	*cwd;
	char	*parent_dir;
	int		is_repo;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (0);
	while (1)
	{
		is_repo = check_git_in_dir(cwd);
		if (is_repo)
		{
			free(cwd);
			return (1);
		}
		if (ft_strcmp(cwd, "/") == 0)
			break ;
		parent_dir = get_parent_dir(cwd);
		free(cwd);
		if (!parent_dir)
			return (0);
		cwd = parent_dir;
	}
	free(cwd);
	return (0);
}

char	*create_prompt(void)
{
	char	*username;
	char	*cwd;
	char	*prompt;
	int		in_git_repo;

	username = get_username();
	cwd = get_current_dir();
	if (!username || !cwd)
		return (NULL);
	in_git_repo = is_git_repo();
	prompt = ft_strdup(COLOR_GREEN);
	prompt = append_to_prompt(prompt, username);
	prompt = append_to_prompt(prompt, COLOR_RESET ":");
	prompt = append_to_prompt(prompt, COLOR_CYAN);
	prompt = append_to_prompt(prompt, cwd);
	if (in_git_repo)
	{
		prompt = append_to_prompt(prompt, COLOR_YELLOW);
		prompt = append_to_prompt(prompt, " (git)");
	}
	prompt = append_to_prompt(prompt, COLOR_RESET " $ ");
	free(username);
	free(cwd);
	return (prompt);
}
