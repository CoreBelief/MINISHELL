/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer_utils.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/30 17:22:38 by eeklund       #+#    #+#                 */
/*   Updated: 2024/07/30 19:40:40 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_redirection(char *input, int *i, t_token **head)
{
	if (input[*i] == '>')
	{
		if (input[*i + 1] == '>')
		{
			add_token(head, strdup(">>"), TOKEN_REDIRECT_APPEND);
			(*i) = (*i) + 2;
		}
		else
		{
			add_token(head, strdup(">"), TOKEN_REDIRECT_OUT);
			(*i)++;
		}
	}
	else if (input[*i] == '<')
	{
		if (input[*i + 1] == '<')
		{
			add_token(head, strdup("<<"), TOKEN_HEREDOC);
			(*i) = (*i) + 2;
		}
		else
		{
			add_token(head, strdup("<"), TOKEN_REDIRECT_IN);
			(*i)++;
		}
	}
}

void	handle_single(char *input, int *i, t_token **head)
{
	int		start;
	char	*content;

	start = *i + 1;
	content = NULL;
	(*i)++;
	while (input[*i] != '\'')
		(*i)++;
	if (input[*i] == '\'')
	{
		content = strndup(&input[start], *i - start);
		add_token(head, content, TOKEN_SINGLE_QUOTE);
		(*i)++;
	}
}

void	handle_double(char *input, int *i, t_token **head)
{
	int		start;
	char	*content;

	start = *i + 1;
	content = NULL;
	(*i)++;
	while (input[*i] != '"')
		(*i)++;
	if (input[*i] == '"')
	{
		content = strndup(&input[start], *i - start);
		add_token(head, content, TOKEN_DOUBLE_QUOTE);
		(*i)++;
	}
}

void	skip_whitespace(char *input, int *i)
{
	while (is_whitespace(input[*i]))
		(*i)++;
}

void	handle_word(char *input, int *i, t_token **head)
{
	int		start;
	char	*content;

	start = *i + 1;
	content = NULL;
	(*i)++;
	while (input[*i] != '\'')
		(*i)++;
	if (input[*i] == '\'')
	{
		content = strndup(&input[start], *i - start);
		add_token(head, content, TOKEN_WORD);
		(*i)++;
	}
}

void	print_token_list(t_token *head)
{
	t_token	*temp;

	
	while (head)
	{
		temp = head;
		head = head->next;
		printf("token: content->%s type->%i, ", temp->content, temp->type);
	}
	printf("\n");
}
