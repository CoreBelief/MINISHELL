/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/30 19:32:27 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/16 17:29:37 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "minishell.h"

static void	print_export_var(char *var);
static void	sort_env(char **sorted_env, int size);
static int	print_sorted_env(t_shell *shell);
void		builtin_export(char **args, t_shell *shell);

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

static int	is_in_export_list(char *var, t_shell *shell)
{
	int		i;
	char	*equal_sign;
	size_t	len;

	equal_sign = ft_strchr(var, '=');
	len = equal_sign ? (size_t)(equal_sign - var) : ft_strlen(var);
	for (i = 0; i < shell->export_size; i++)
	{
		if (ft_strncmp(shell->export_list[i], var, len) == 0
			&& (shell->export_list[i][len] == '='
			|| shell->export_list[i][len] == '\0'))
			return (1);
	}
	return (0);
}

static void	print_and_free_env(char **combined_env, int combined_size)
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

static int	print_sorted_env(t_shell *shell)
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
	print_and_free_env(combined_env, combined_size);
	return (1);
}

// static int print_sorted_env(t_shell *shell)
// {
//     char **combined_env;
//     int combined_size;
//     int i, j;

//     // Calculate the total size (export_list + env)
//     combined_size = shell->export_size + shell->env_size;

//     // Allocate memory for the combined list
//     combined_env = malloc(sizeof(char *) * (combined_size + 1));
//     if (!combined_env)
//         return (0);

//     // Copy export_list
//     for (i = 0; i < shell->export_size; i++)
//         combined_env[i] = ft_strdup(shell->export_list[i]);

//     // Copy env (excluding duplicates)
//     for (i = 0, j = shell->export_size; i < shell->env_size; i++)
//     {
//         if (!is_in_export_list(shell->env[i], shell))
//             combined_env[j++] = ft_strdup(shell->env[i]);
//     }

//     combined_env[j] = NULL;
//     combined_size = j;

//     // Sort the combined list
//     sort_env(combined_env, combined_size);

//     // Print the sorted list
//     for (i = 0; i < combined_size; i++)
//         print_export_var(combined_env[i]);

//     // Free the combined list
//     for (i = 0; i < combined_size; i++)
//         free(combined_env[i]);
//     free(combined_env);

//     return (1);
// }

void	builtin_export(char **args, t_shell *shell)
{
	int		i;
	char	*equal_sign;

	i = 1;
	if (!args[1])
	{
		if (!print_sorted_env(shell))
		{
			shell->last_exit_status = 1;
			return ;
		}
	}
	while (args[i])
	{
		equal_sign = ft_strchr(args[i], '=');
		if (!process_identifier(args[i], equal_sign, shell))
			return ;
		i++;
	}
	shell->last_exit_status = 0;
}
