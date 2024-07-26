#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>


char *find_command_path(char *command)
{
    char *path = getenv("PATH");
    char *path_copy = strdup(path);
    char *dir = strtok(path_copy, ":");
    char *full_path = NULL;

    while (dir != NULL) {
        full_path = malloc(strlen(dir) + strlen(command) + 2);
        sprintf(full_path, "%s/%s", dir, command);
        
        if (access(full_path, X_OK) == 0) {
            free(path_copy);
            return full_path;
        }
        
        free(full_path);
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return strdup(command);  // Return the command as-is if not found in PATH
}

t_command *parse_command(char *input)
{
    t_command *cmd;
    char **tokens;

    cmd = malloc(sizeof(t_command));
    if (!cmd)
        return NULL;

    // Initialize all fields to NULL or 0
    cmd->path = NULL;
    cmd->argv = NULL;
    cmd->redirects = NULL;
    cmd->redirect_count = 0;
    cmd->pipe_in = STDIN_FILENO;
    cmd->pipe_out = STDOUT_FILENO;

    tokens = tokenize_command(input);
    if (!tokens)
    {
        free(cmd);
        return NULL;
    }

    cmd->path = find_command_path(tokens[0]);
    cmd->argv = tokens;

    return cmd;
}