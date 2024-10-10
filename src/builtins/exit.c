/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exit.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/09 17:25:25 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/10 17:14:40 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static int is_space(char c)
// {
//     return (c == ' ' || (c >= '\t' && c <= '\r'));
// }
static int	is_space(char c)
{
	return (c == ' ' || (c >= '\t' && c <= '\r'));
}

static int	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

// // Trims leading and trailing spaces
// static char *trim_whitespace(char *str) {
//     char *end;

//     // Trim leading space
//     while (is_space(*str)) str++;

//     if (*str == 0)  // If string is all spaces
//         return (str);

//     // Trim trailing space
//     end = str + strlen(str) - 1;
//     while (end > str && is_space(*end)) end--;

//     // Write new null terminator
//     *(end + 1) = '\0';

//     return (str);
// }
static char	*trim_whitespace(char *str)
{
	char	*end;

	// Trim leading space
	while (is_space(*str))
		str++;
	if (*str == 0) // If string is all spaces
		return (str);
	// Trim trailing space
	end = str + strlen(str) - 1;
	while (end > str && is_space(*end))
		end--;
	// Write new null terminator
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
	str = trim_whitespace((char *)str);
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] != '\0')
	{
		if (!is_digit(str[i]))
			return (0);
		if (result > (ULONG_MAX - (str[i] - '0')) / 10)
			return (0);
		result = result * 10 + (str[i] - '0');
		i++;
	}
	if ((sign == -1 && result > (unsigned long)LONG_MAX + 1) || (sign == 1
			&& result > (unsigned long)LONG_MAX))
		return (0);
	*number = sign * (long)result;
	return (1);
}

// void builtin_exit(char **args)
// {
//     long exit_status;
//     int numeric_status = 0;

//     printf("exit\n");
//     if (!args[1])
//         exit(0);
// 		numeric_status = is_valid_number(args[1], &exit_status);
//     if (!numeric_status)
//     {
//         ft_putstr_fd("exit: numeric argument required\n", 2);
//         exit(2);
//     }
//     if (args[2])
//     {
//         ft_putstr_fd("exit: too many arguments\n", 2);
//         exit(1);
//     }

//     exit((int)(exit_status & 255));
// }
void	builtin_exit(char **args)
{
	long	exit_status;
	int		numeric_status;
	char	*trimmed_arg;

	printf("exit\n");
	// printf("args == %s\n", args[1]);
	// If no argument is provided, exit with status 0
	if (!args[1])
		exit(0);
	// Trim whitespace from the argument
	trimmed_arg = trim_whitespace(args[1]);
	// Debugging print to see what trimmed_arg looks like
	// printf("Trimmed argument: '%s'\n", trimmed_arg);
	// Check if the trimmed argument is an empty string
	if (strlen(trimmed_arg) == 0)
	{
		ft_putstr_fd("exit: numeric argument required\n", 2);
		exit(2);
	}
	// Validate if the argument is a valid number
	numeric_status = is_valid_number(trimmed_arg, &exit_status);
	// If the argument is not a valid number, exit with status 2
	if (!numeric_status)
	{
		ft_putstr_fd("exit: numeric argument required\n", 2);
		exit(2);
	}
	// If there are more than one argument, show an error and exit with status 1
	if (args[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		exit(1);
	}
	// Exit with the valid exit status (masked to 8 bits)
	exit((int)(exit_status & 255));
}
