/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export_print.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/17 12:10:48 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/17 12:25:39 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_in_export_list(char *var, t_shell *shell);
static void	print_export_var(char *var);
static void	print_and_free_export(char **combined_env, int combined_size);
static void	sort_env(char **sorted_env, int size);
int			print_export(t_shell *shell);

static int	is_in_export_list(char *var, t_shell *shell)
{
	int		i;
	char	*equal_sign;
	size_t	len;

	equal_sign = ft_strchr(var, '=');
	if (equal_sign)
		len = (size_t)(equal_sign - var);
	else
		len = ft_strlen(var);
	i = 0;
	while (i < shell->export_size)
	{
		if (ft_strncmp(shell->export_list[i], var, len) == 0
			&& (shell->export_list[i][len] == '='
			|| shell->export_list[i][len] == '\0'))
			return (1);
		i++;
	}
	return (0);
}

static void	print_export_var(char *var)
{
	char	*equal_sign;

	equal_sign = ft_strchr(var, '=');
	if (equal_sign)
	{
		printf("declare -x %.*s\"%s\"\n", (int)(equal_sign - var + 1), var,
			equal_sign + 1);
	}
	else
	{
		printf("declare -x %s\n", var);
	}
}

static void	print_and_free_export(char **combined_env, int combined_size)
{
	int	i;

	i = -1;
	while (++i < combined_size)
	{
		print_export_var(combined_env[i]);
		free(combined_env[i]);
	}
	free(combined_env);
}

static void	sort_env(char **sorted_env, int size)
{
	int		i;
	int		j;
	char	*temp;

	i = -1;
	while (++i < size - 1)
	{
		j = i;
		while (++j < size)
		{
			if (ft_strcmp(sorted_env[i], sorted_env[j]) > 0)
			{
				temp = sorted_env[i];
				sorted_env[i] = sorted_env[j];
				sorted_env[j] = temp;
			}
		}
	}
}

int	print_export(t_shell *shell)
{
	char	**combined_env;
	int		combined_size;
	int		i;
	int		j;

	combined_size = shell->export_size + shell->env_size;
	combined_env = malloc(sizeof(char *) * (combined_size + 1));
	if (!combined_env)
		return (0);
	i = -1;
	while (++i < shell->export_size)
		combined_env[i] = ft_strdup(shell->export_list[i]);
	i = -1;
	j = shell->export_size;
	while (++i < shell->env_size)
	{
		if (!is_in_export_list(shell->env[i], shell))
			combined_env[j++] = ft_strdup(shell->env[i]);
	}
	combined_env[j] = NULL;
	combined_size = j;
	sort_env(combined_env, combined_size);
	print_and_free_export(combined_env, combined_size);
	return (1);
}
