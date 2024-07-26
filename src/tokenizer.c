#include "minishell.h"
#include <string.h>
#include <stdio.h>

/// 
/// test tokenize (TODO MAKE REAL LEGAL TOKENiZER)
char **tokenize_command(char *command)
{
    char **args;
    char *token;
    int i;

    printf("DEBUG: Entering tokenize_command function\n");
    printf("DEBUG: Input command: '%s'\n", command);

    args = malloc(sizeof(char *) * MAX_ARGS);
    if (!args)
    {
        printf("DEBUG: Memory allocation failed for args\n");
        return (NULL);
    }
    printf("DEBUG: Memory allocated for args\n");

    i = 0;
    token = strtok(command, " ");
    printf("DEBUG: First token: '%s'\n", token);

    while (token && i < MAX_ARGS - 1)
    {
        args[i] = token;
        printf("DEBUG: Token %d: '%s'\n", i, args[i]);
        i++;
        token = strtok(NULL, " ");
    }

    args[i] = NULL;
    printf("DEBUG: Total tokens: %d\n", i);

    if (i == MAX_ARGS - 1 && token)
    {
        printf("DEBUG: Warning: Maximum number of arguments reached. Some input may be truncated.\n");
    }

    printf("DEBUG: Exiting tokenize_command function\n");
    return (args);
}