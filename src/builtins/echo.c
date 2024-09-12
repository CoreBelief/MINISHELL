#include "builtins.h"
#include "minishell.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

static bool	is_only_n_flag(char *arg);
void 		builtin_echo(char **args);


static bool	is_only_n_flag(char *arg)
{
	int	i;

	if (arg[0] != '-')
		return (false);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}


void builtin_echo(char **args)
{
	int i = 1;
	bool newline = true;
	if (!args)
		return;
	while (args[i] && is_only_n_flag(args[i]))
	{	
		
		newline = false;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
}
