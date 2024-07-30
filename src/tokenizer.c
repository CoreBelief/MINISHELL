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


int	is_whitespace(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (0);
	return (1);
}

t_token	*tokenizer(char *input)
{
	t_token	*head;
	int		i;

	printf("entered tokenizer\n");
	head = NULL;
	i = 0;
	while (input[i])
	{
		if (input[i] == '|')
			add_token(&head, strdup("|"), TOKEN_PIPE);
		else if (input[i] == '<' || input[i] == '>')
			handle_redirection(input, &i, &head);
		else if (input[i] == '\'')
			handle_single(&input[i], &i, &head);
		else if (input[i] == '"')
			handle_double(&input[i], &i, &head);
		else if (is_whitespace(input[i]))
			i++;
		else
			handle_word(&input[i], &i, &head);
		printf("index in tokenizer %i\n", i);
	}
	return (head);
}


t_token	*add_token(t_token **head, char *content, t_token_type type)
{
	t_token	*new_token;
	t_token	*current;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->content = content;
	new_token->type = type;
	new_token->next = NULL;
	if (*head == NULL)
		*head = new_token;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new_token;
	}
	return (new_token);
}
