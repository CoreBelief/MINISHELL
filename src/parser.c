#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>
#include "tokenizer.h"


char *find_command_path(char *command)
{
    char *path = getenv("PATH");
    char *path_copy = ft_strdup(path);
    //illeagal strtok!!!
    char *dir = strtok(path_copy, ":");
    char *full_path = NULL;

    while (dir != NULL) {
        full_path = malloc(strlen(dir) + ft_strlen(command) + 2);
        sprintf(full_path, "%s/%s", dir, command);
        
        if (access(full_path, X_OK) == 0) {
            free(path_copy);
            return full_path;
        }
        
        free(full_path);
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return ft_strdup(command);  // Return the command as-is if not found in PATH
}


t_command *parse_command(char *input)
{
    t_command *cmd;
    t_token *tokens;
    int argc = 0;
    int i;

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

    tokens = tokenizer(input);
    if (!tokens)
    {
        free(cmd);
        return NULL;
    }
    print_token_list(tokens);
    // Count the number of word tokens to determine argc
    for (t_token *t = tokens; t != NULL; t = t->next)
    {
        if (t->type == TOKEN_WORD)
            argc++;
    }

    cmd->argv = malloc(sizeof(char *) * (argc + 1));
    if (!cmd->argv)
    {
        free(cmd);
        free_tokens(&tokens);
        return NULL;
    }

    // Fill argv array
    i = 0;
    for (t_token *t = tokens; t != NULL; t = t->next)
    {
        if (t->type == TOKEN_WORD)
        {
            cmd->argv[i] = ft_strdup(t->content);
            i++;
        }
        // Here you can handle redirections and other token types
    }
    cmd->argv[i] = NULL;

    if (argc > 0)
        cmd->path = find_command_path(cmd->argv[0]);

    free_tokens(&tokens);
    return cmd;
}