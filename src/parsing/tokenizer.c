/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/28 12:19:48 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/28 13:57:40 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_whitespace(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

int	tokenize_pipe(int *i, t_token **head)
{
	if (*i == 0)  // invalid input if it starts with pipe, i think there might be a better solution later in parser maybe
	{
		printf("syntex error, pipe");
		return (0);
	}
	add_token(head, ft_strdup("|"), TOKEN_PIPE);
	(*i)++;
	return (1);
}

// have to fix if there are 2 consecutive commands in handle_word function
t_token	*tokenizer(char *input, t_shell *shell)
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
			if (!tokenize_pipe(&i, &head))
				break ;
		}
		else if (input[i] == '<' || input[i] == '>')
			tokenize_redirection(input, &i, &head);
		else
			tokenize_word(input, &i, &head, shell);
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
