/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer_utils.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/30 17:22:38 by eeklund       #+#    #+#                 */
/*   Updated: 2024/08/30 16:24:21 by elleneklund   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include "libft.h"


void	tokenize_redirection(char *input, int *i, t_token **head)
{
	// printf("entered handle_redirection\n");
	if (input[*i] == '>')
	{
		if (input[*i + 1] == '>')
		{
			add_token(head, ft_strdup(">>"), TOKEN_REDIRECT_APPEND);
			(*i) = (*i) + 2;
		}
		else
		{
			add_token(head, ft_strdup(">"), TOKEN_REDIRECT_OUT);
			(*i)++;
		}
	}
	else if (input[*i] == '<')
	{
		if (input[*i + 1] == '<')
		{
			add_token(head, ft_strdup("<<"), TOKEN_HEREDOC);
			(*i) = (*i) + 2;
		}
		else
		{
			add_token(head, ft_strdup("<"), TOKEN_REDIRECT_IN);
			(*i)++;
		}
	}
}

void	tokenize_single(char *input, int *i, t_token **head)
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
		content = ft_strndup(&input[start], *i - start);
		add_token(head, content, TOKEN_SINGLE_QUOTE);
		(*i)++;
	}
	// else its a wrong input??
}

void	tokenize_double(char *input, int *i, t_token **head)
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
		content = ft_strndup(&input[start], *i - start);
		add_token(head, content, TOKEN_DOUBLE_QUOTE);
		(*i)++;
	}
	// else its a wrong input??
}

bool is_special_token(char c)
{
    return (c == '|' || c == '<' || c == '>');
}
//the special token thing fucks it up if there are 2 consecutive commands or not????
//smth weird might not be here 
void	tokenize_word(char *input, int *i, t_token **head)
{
	int		start;
	char	*content;

	start = *i;
	content = NULL;
	// printf("entered handle_word, word is==%s & index is ==%i\n", input, *i);
	while (input[*i] && !is_whitespace(input[*i]) && !is_special_token(input[*i]) && input[*i] != '\'' && input[*i] != '"')
		(*i)++;
	// printf("index afterrr in handle word %i\n", *i);
	content = ft_strndup(&input[start], *i - start);
	// printf("word:%s\n", content);
	add_token(head, content, TOKEN_WORD);	
	if (!is_special_token(input[*i]) && input[*i] != '\'' && input[*i] != '"')
		(*i)++;
}

void	print_token_list(t_token *head)
{
	t_token	*temp;

	printf("inside print token_lst\n");
	while (head)
	{
		temp = head;
		head = head->next;
		printf("TOKEN-> '%s' type:%i\n", temp->content, temp->type);
	}
	printf("\n");
}
