/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer_utils.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/30 17:22:38 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/19 14:31:32 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include "minishell.h"


void	tokenize_redirection(char *input, int *i, t_token **head)
{
	if (input[*i] == '>' || input[*i] == '<')
	{
		if (input[*i + 1] == input[*i])
		{
			if (input[*i] == '>')
				add_token(head, ft_strdup(">>"), TOKEN_REDIRECT_APPEND);
			else
				add_token(head, ft_strdup("<<"), TOKEN_HEREDOC);
			(*i) = (*i) + 2;
		}
		else
		{
			if (input[*i] == '>')
				add_token(head, ft_strdup(">"), TOKEN_REDIRECT_OUT);
			else
				add_token(head, ft_strdup("<"), TOKEN_REDIRECT_IN);
			(*i)++;
		}
	}
}

void	tokenize_single(char *input, int *i, t_token **head)
{
	int		start;
	char	*content;

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
}

void	tokenize_double(char *input, int *i, t_token **head)
{
	int		start;
	char	*content;

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
// 	else // to print if unclosed ", not sure how we want to handle this case
// 	{
// 		start--;
// 		content = ft_strndup(&input[start], *i - start);
// 		add_token(head, content, TOKEN_DOUBLE_QUOTE);
// 		(*i)++;
// 	}
}

bool	is_special_token(char c)
{
	return (c == '|' || c == '<' || c == '>');
}


//the special token thing fucks it up if there are 2 consecutive commands or not????
//smth weird might not be here 


// void	tokenize_word(char *input, int *i, t_token **head)
// {
// 	int		start;
// 	char	*content;

// 	start = *i;
// 	content = NULL;
// 	while (input[*i] && !is_whitespace(input[*i]) && 
// 	!is_special_token(input[*i]) && input[*i] != '\'' && input[*i] != '"')
// 		(*i)++;
// 	content = ft_strndup(&input[start], *i - start);
// 	add_token(head, content, TOKEN_WORD);
// 	if (!is_special_token(input[*i]) && input[*i] != '\'' && input[*i] != '"')
// 		(*i)++;
// }

void	tokenize_word(char *input, int *i, t_token **head)
{
	int start = *i;
	int in_quotes = 0;
	char quote_char = 0;

	while (input[*i])
	{
		if (input[*i] == '"' || input[*i] == '\'')
		{
			if (!in_quotes)
			{
				in_quotes = 1;
				quote_char = input[*i];
			}
			else if (input[*i] == quote_char)
			{
				in_quotes = 0;
				quote_char = 0;
			}
		}
		else if (!in_quotes && (is_whitespace(input[*i]) || is_special_token(input[*i])))
		{
			break ;
		}
		(*i)++;
	}

	char *content = ft_strndup(&input[start], *i - start);
	add_token(head, content, TOKEN_WORD);
}

// void	tokenize_word(char *input, int *i, t_token **head)
// {
// 	int		start;
// 	char	*content;

// 	start = *i;
// 	content = NULL;
// 	// printf("entered handle_word, word is==%s & index is ==%i\n", input, *i);
// 	while (input[*i] && !is_whitespace(input[*i]) && !is_special_token(input[*i]) && input[*i] != '\'' && input[*i] != '"')
// 		(*i)++;
// 	// printf("index afterrr in handle word %i\n", *i);
// 	content = ft_strndup(&input[start], *i - start);
// 	// printf("word:%s\n", content);
// 	add_token(head, content, TOKEN_WORD);	
// 	if (!is_special_token(input[*i]) && input[*i] != '\'' && input[*i] != '"')
// 		(*i)++;
// }

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
