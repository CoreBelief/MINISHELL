#include "tokenizer.h"
#include "minishell.h"


int	is_whitespace(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}
// have to fix if there are 2 consecutive commands in handle_word function
t_token	*tokenizer(char *input)
{
	t_token	*head;
	int		i;
	int		len;

	head = NULL;
	i = 0;
	len = ft_strlen(input);
	while (input[i] != '\0' && i < len)
	{
		// printf("entered and index is==%i and it is ==%s  input[i]==%c\n", i, input, input[i]);
		while (is_whitespace(input[i]))
			i++;
		if (input[i] == '\0')
			break ;
		if (input[i] == '|')
		{
			add_token(&head, ft_strdup("|"), TOKEN_PIPE);
			i++;
		}
		else if (input[i] == '<' || input[i] == '>')
			tokenize_redirection(input, &i, &head);
		else if (input[i] == '\'')
			tokenize_single(input, &i, &head);
		else if (input[i] == '"')
			tokenize_double(input, &i, &head);
		else
			tokenize_word(input, &i, &head);
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

void	free_tokens(t_token **head)
{
	t_token	*temp;

	if (!head)
		return ;
	while (*head)
	{
		temp = *head;
		*head = (*head)->next;
		free(temp);
	}
}
