/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer_utils.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/30 17:22:38 by eeklund       #+#    #+#                 */
/*   Updated: 2024/07/31 14:39:47 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_redirection(char *input, int *i, t_token **head)
{
	// printf("entered handle_redirection\n");
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

	// printf("entered handle_single\n");
	start = *i + 1;
	content = NULL;
	(*i)++;
	while (input[*i] != '\'' && input[*i])
		(*i)++;
	if (input[*i] == '\'')
	{
		content = strndup(&input[start], *i - start);
		add_token(head, content, TOKEN_SINGLE_QUOTE);
		(*i)++;
	}
	// else its a wrong input??
}

void	handle_double(char *input, int *i, t_token **head)
{
	int		start;
	char	*content;

	// printf("entered handle_double\n");
	start = *i + 1;
	content = NULL;
	(*i)++;
	while (input[*i] != '"' && input[*i])
		(*i)++;
	if (input[*i] == '"')
	{
		content = strndup(&input[start], *i - start);
		add_token(head, content, TOKEN_DOUBLE_QUOTE);
		(*i)++;
	}
	// else its a wrong input??
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

	start = *i;
	content = NULL;
	// printf("entered handle_word & index is %i\n", *i);
	while (!is_whitespace(input[*i]) && input[*i])
		(*i)++;
	// printf("index in handle word %i\n", *i);
	// if (input[*i])
	// {
	// 	if (is_whitespace(input[*i]))
	// 	{
			content = strndup(&input[start], *i - start);
			// printf("word:%s\n", content);
			add_token(head, content, TOKEN_WORD);
			(*i)++;
	// 	}
	// }
}

void	print_token_list(t_token *head)
{
	t_token	*temp;

	printf("inside print token_lst\n");
	while (head)
	{
		temp = head;
		head = head->next;
		printf("token: content->%s type->%i, ", temp->content, temp->type);
	}
	printf("\n");
}
