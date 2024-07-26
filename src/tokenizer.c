#include "minishell.h"

char **tokenize_command(char *command)
{
    char **args;
    char *token;
    int i;

    args = malloc(sizeof(char *) * MAX_ARGS);
    if (!args)
        return NULL;

    i = 0;
    token = strtok(command, " ");
    while (token && i < MAX_ARGS - 1)
    {
        args[i] = strdup(token);  // Use strdup to allocate new memory for each token
        if (!args[i])
        {
            // Free previously allocated memory if strdup fails
            while (--i >= 0)
                free(args[i]);
            free(args);
            return NULL;
        }
        i++;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    return args;
}