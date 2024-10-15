/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishellception.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/14 13:38:47 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/14 15:14:15 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		init_shell(t_shell *shell);
void		free_shell(t_shell *shell);
int			increment_shlvl(t_shell *shell);

void	init_shell(t_shell *shell)
{
	shell->tokens = NULL;
	shell->commands = NULL;
	shell->last_exit_status = 0;
	shell->env = NULL;
	shell->env_size = 0;
	shell->export_list = NULL;
	shell->export_size = 0;
}

void	free_shell(t_shell *shell)
{
	if (shell->env)
	{
		ft_free_str_array(shell->env);
	}
	if (shell->export_list)
	{
		ft_free_str_array(shell->export_list);
	}
}

int	increment_shlvl(t_shell *shell)
{
	char	*shlvl_str;
	char	*new_shlvl;
	int		shlvl;

	shlvl = 1;
	shlvl_str = ft_get_env("SHLVL", shell);
	if (shlvl_str != NULL)
	{
		shlvl = ft_atoi(shlvl_str);
		shlvl += 1;
	}
	new_shlvl = ft_itoa(shlvl);
	if (!ft_set_env("SHLVL", new_shlvl, shell))
	{
		free(new_shlvl);
		return (0);
	}
	free(new_shlvl);
	return (1);
}
