#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "minishell.h"


static int is_space(char c) 
{
    return (c == ' ' || (c >= '\t' && c <= '\r'));
}

static int is_digit(char c) 
{
    return (c >= '0' && c <= '9');
}

// Trims leading and trailing spaces
static char *trim_whitespace(char *str) {
    char *end;

    // Trim leading space
    while (is_space(*str)) str++;

    if (*str == 0)  // If string is all spaces
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && is_space(*end)) end--;

    // Write new null terminator
    *(end + 1) = '\0';

    return str;
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
	if ((sign == -1 && result > (unsigned long)LONG_MAX + 1)
		|| (sign == 1 && result > (unsigned long)LONG_MAX))
		return (0);
	*number = sign * (long)result;
	return (1);
}

void builtin_exit(char **args) 
{
    long exit_status;
    int numeric_status = 0;

    printf("exit\n");
    if (!args[1]) 
        exit(0);
	  numeric_status = is_valid_number(args[1], &exit_status);
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


// #include <limits.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <ctype.h>
// #include <string.h>
// #include "minishell.h"

// static int	is_space(char c);
// static int	is_digit(char c);
// static int	is_valid_number(const char *str, long *number);
// void		builtin_exit(char **args);


// static int	is_valid_number(const char *str, long *number)
// {
// 	int				i;
// 	int				sign;
// 	unsigned long	result;

// 	i = 0;
// 	sign = 1;
// 	result = 0;
// 	while (is_space(str[i]))
// 		i++;
// 	if (str[i] == '+' || str[i] == '-')
// 	{
// 		if (str[i] == '-')
// 			sign = -1;
// 		i++;
// 	}
// 	if (!is_digit(str[i]))
// 		return (0);
// 	while (str[i] != '\0')
// 	{
// 		if (!is_digit(str[i]))
// 			return (0);
// 		if (result > (ULONG_MAX - (str[i] - '0')) / 10)
// 			return (0);
// 		result = result * 10 + (str[i] - '0');
// 		i++;
// 	}
// 	if ((sign == -1 && result > (unsigned long)LONG_MAX + 1)
// 		|| (sign == 1 && result > (unsigned long)LONG_MAX))
// 		return (0);
// 	*number = sign * (long)result;
// 	return (1);
// }

// void builtin_exit(char **args) 
// {
//     long exit_status;
//     int numeric_status = 0;

//     printf("exit\n");
//     if (!args[1]) 
//         exit(0);
//     if (args[2]) 
//     {
//         fprintf(stderr, "minishell: exit: too many arguments\n");
//         exit(1);
//     }
//     numeric_status = is_valid_number(args[1], &exit_status);
//     if (!numeric_status) 
//     {
//         fprintf(stderr, "minishell: exit: %s: numeric argument required\n", args[1]);
//         exit(2);
//     }
//     exit((int)(exit_status & 255));
// }