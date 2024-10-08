/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pwd.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/04 13:54:14 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/04 13:54:24 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_pwd(char **args, t_shell *shell);

void	builtin_pwd(char **args, t_shell *shell)
{
	char	*cwd;

	(void)args;
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		printf("%s\n", cwd);
		free(cwd);
		shell->last_exit_status = 0;
	}
	else
	{
		perror("pwd");
		shell->last_exit_status = 1;
	}
}
