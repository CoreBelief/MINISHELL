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


t_token *tokenizer(char *input)
{
    t_token *head;
    int     i;

    head = NULL;
    i = 0;
    while (input[i])
    {
/* quotes, double quotes, redirections, greater that, smallaer than, text
skipping whitespace
*/
    }
    return (head);
}


t_token *add_token(t_token **head, char *content, token_type type)
{
    t_token *new_token;
    
    new_token = malloc(sizeof(t_token));
    if (!new_token)
        return NULL;
    new_token->content = content;
    new_token->type = type;
    new_token->next = NULL;
    if (*head == NULL)
        *head = new_token;
    else
    {
        t_token *current = *head;
        while (current->next)
            current = current->next;
        current->next = new_token;
    }
    return new_token;
}