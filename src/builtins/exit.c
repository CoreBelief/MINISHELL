/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exit.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/09 17:25:25 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/15 18:26:02 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*trim_whitespace(char *str);
static int	is_valid_number(const char *str, long *number);
void		builtin_exit(char **args);


static char	*trim_whitespace(char *str)
{
	char	*end;

	while (is_whitespace(*str))
		str++;
	if (*str == 0)
		return (str);
	end = str + strlen(str) - 1;
	while (end > str && is_whitespace(*end))
		end--;
	*(end + 1) = '\0';
	return (str);
}

static int	is_valid_number(const char *str, long *number)
{
	int				i;
	int				sign;
	unsigned long	result;

	i = 0;
	sign = 1;
	result = 0;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] != '\0')
	{
		if (!ft_isdigit(str[i]) || result > (ULONG_MAX - (str[i] - '0')) / 10)
			return (0);
		result = result * 10 + (str[i] - '0');
		i++;
	}
	if ((sign == -1 && result > (unsigned long)LONG_MAX + 1)
		|| (sign == 1 && result > (unsigned long)LONG_MAX))
		return (0);
	*number = sign * (long)result;
	return (1);
}

void	builtin_exit(char **args)
{
	long	exit_status;
	int		numeric_status;
	char	*trimmed_arg;

	printf("exit\n");
	if (!args[1])
		exit(0);
	trimmed_arg = trim_whitespace(args[1]);
	if (strlen(trimmed_arg) == 0)
	{
		ft_putstr_fd("exit: numeric argument required\n", 2);
		exit(2);
	}
	numeric_status = is_valid_number(trimmed_arg, &exit_status);
	if (!numeric_status)
	{
		ft_putstr_fd("exit: numeric argument required\n", 2);
		exit(2);
	}
	if (args[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		exit(1);
	}
	exit((int)(exit_status & 255));
}
