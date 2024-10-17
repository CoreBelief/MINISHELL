/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/30 19:32:27 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/17 12:25:29 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "minishell.h"

static int	set_environment(char *arg, char *equal_sign, t_shell *shell);
static int	process_identifier(char *arg, char *equal_sign, t_shell *shell);
void		builtin_export(char **args, t_shell *shell);

static int	set_environment(char *arg, char *equal_sign, t_shell *shell)
{
	char	*name;
	char	*value;
	int		append_mode;

	name = arg;
	value = NULL;
	append_mode = 0;
	if (equal_sign)
	{
		*equal_sign = '\0';
		value = equal_sign + 1;
		if (equal_sign > arg && *(equal_sign - 1) == '+')
		{
			append_mode = 1;
			*(equal_sign - 1) = '\0';
		}
	}
	if (!process_environment(name, value, append_mode, shell))
		return (0);
	restore_equal_sign(equal_sign, append_mode);
	return (1);
}

static int	process_identifier(char *arg, char *equal_sign, t_shell *shell)
{
	char	*identifier;
	int		result;

	if (equal_sign != NULL)
	{
		if (!handle_equal_sign(arg, equal_sign, &identifier))
		{
			shell->last_exit_status = 1;
			return (0);
		}
	}
	else
		handle_no_equal_sign(arg, &identifier);
	if (!validate_identifier(identifier, arg, shell))
	{
		if (equal_sign != NULL)
			free(identifier);
		return (0);
	}
	result = set_environment(arg, equal_sign, shell);
	if (!result)
		shell->last_exit_status = 1;
	if (equal_sign != NULL)
		free(identifier);
	return (result);
}

void	builtin_export(char **args, t_shell *shell)
{
	int		i;
	char	*equal_sign;

	i = 1;
	if (!args[1])
	{
		if (!print_export(shell))
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

// static int print_export(t_shell *shell)
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