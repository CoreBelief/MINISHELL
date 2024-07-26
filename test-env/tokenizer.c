#include "minishell.h"
#include <string.h>


/// 
/// test tokenize (TODO MAKE REAL LEGAL TOKENiZER)
char	**tokenize_command(char *command)
{
	char	**args;
	char	*token;
	int		i;

	args = malloc(sizeof(char *) * MAX_ARGS);
	if (!args)
		return (NULL);
	i = 0;
	token = strtok(command, " ");
	while (token && i < MAX_ARGS - 1)
	{
		args[i++] = token;
		token = strtok(NULL, " ");
	}
	args[i] = NULL;
	return (args);
}