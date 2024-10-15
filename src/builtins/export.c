/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/30 19:32:27 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/14 14:39:48 by eeklund       ########   odam.nl         */
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
	char	*e_sign;

	e_sign = ft_strchr(var, '=');
	if (e_sign)
		printf("declare -x %.*s\"%s\"\n", (int)(e_sign - var + 1),
			var, e_sign + 1);
	else
		printf("declare -x %s\n", var);
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

// static int	print_sorted_env(t_shell *shell)
// {
// 	char	**sorted_env;
// 	int		i;
// 	char	*e_sign;

// 	sorted_env = malloc(sizeof(char *) * (shell->env_size + 1));
// 	if (!sorted_env)
// 		return (0);
// 	i = -1;
// 	while (++i < shell->env_size)
// 		sorted_env[i] = shell->env[i];
// 	sorted_env[i] = NULL;
// 	sort_env(sorted_env, shell->env_size);
// 	i = -1;
// 	while (++i < shell->env_size)
// 	{
// 		e_sign = ft_strchr(sorted_env[i], '=');
// 		printf("declare -x %.*s\"%s\"\n", (int)(e_sign - sorted_env[i] + 1),
// 			sorted_env[i], e_sign + 1);
// 	}
// 	free(sorted_env);
// 	return (1);
// }

static int	print_sorted_env(t_shell *shell)
{
	char	**sorted_env;
	int		i;

	sorted_env = malloc(sizeof(char *) * (shell->export_size + 1));
	if (!sorted_env)
		return (0);
	i = -1;
	while (++i < shell->export_size)
		sorted_env[i] = shell->export_list[i];
	sorted_env[i] = NULL;
	sort_env(sorted_env, shell->export_size);
	i = -1;
	while (++i < shell->export_size)
		print_export_var(sorted_env[i]);
	free(sorted_env);
	return (1);
}

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
