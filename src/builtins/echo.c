#include "builtins.h"
#include "minishell.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void builtin_echo(char **args)
{
	if (!args)
		return ;

	int i = 1;
	bool newline = true;

	// Handle multiple -n flags
	while (args[i] && ft_strcmp(args[i], "-n") == 0)
	{
		newline = false;
		i++;
	}

	// Print arguments
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
