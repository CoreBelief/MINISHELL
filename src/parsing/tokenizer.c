#include "tokenizer.h"
#include "minishell.h"


int	is_whitespace(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

int	tokenize_pipe(int *i, t_token **head)
{
	if (*i == 0) 
	{
		printf("syntax error near unexpected token `|'\n");
		return (0); //here we should return an error code (2)?
	}
	add_token(head, ft_strdup("|"), TOKEN_PIPE);
	(*i)++;
	return (1);
}

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
		while (is_whitespace(input[i]))
			i++;
		if (input[i] == '\0')
			break ;
		if (input[i] == '|')
		{
			if (!tokenize_pipe(&i, &head))
				break ;
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
	{
		free (content);
		// content = NULL;
		return (NULL);
	}
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

