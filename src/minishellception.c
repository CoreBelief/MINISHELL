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
static int	countnum(long n);
char		*ft_itoa(int n);
int			increment_shlvl(t_shell *shell);

void	init_shell(t_shell *shell)
{
	shell->tokens = NULL;
	shell->commands = NULL;
	shell->last_exit_status = 0;
	shell->env = NULL;
	shell->env_size = 0;
}

void	free_shell(t_shell *shell)
{
	if (shell->env)
	{
		ft_free_str_array(shell->env);
	}
}

static int	countnum(long n)
{
	int	count;

	if (n == 0)
		return (1);
	count = 0;
	if (n < 0)
	{
		count++;
		n = -n;
	}
	while (n > 0)
	{
		n = n / 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char	*intstring;
	int		len;
	long	num;
	int		i;

	num = (long)n;
	len = countnum(num);
	intstring = (char *)malloc((len + 1) * (sizeof(char)));
	if (!intstring)
		return (NULL);
	i = 0;
	if (num < 0)
		num = -num;
	intstring[len] = '\0';
	while (i < len)
	{
		intstring[len - 1 - i] = (num % 10) + '0';
		num = num / 10;
		i++;
	}
	if (n < 0)
		intstring[0] = '-';
	return (intstring);
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
