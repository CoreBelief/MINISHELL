/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer_utils.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/30 17:22:38 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/28 16:56:53 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include "minishell.h"

// maybe needs to return smth indicating error
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

// maybe needs to return smth indicating error
// void tokenize_word(char *input, int *i, t_token **head)
// {
// 	int start = *i;
// 	int in_quotes = 0;
// 	char quote_char = 0;
// 	char *content;

// 	while (input[*i])
// 	{
// 		if (input[*i] == '"' || input[*i] == '\'')
// 		{
// 			if (!in_quotes)
// 			{
// 				in_quotes = 1;
// 				quote_char = input[*i];
// 			}
// 			else if (input[*i] == quote_char)
// 			{
// 				in_quotes = 0;
// 				quote_char = 0;
// 			}
// 		}
// 		else if (!in_quotes && (is_whitespace(input[*i]) || is_special_token(input[*i])))
// 		{
// 			break ;
// 		}
// 		(*i)++;
// 	}
// 	content = ft_strndup(&input[start], *i - start);
// 	if (!content)
// 		return ;
// 	add_token(head, content, TOKEN_WORD);
// 		// proper error handling if malloc fails in add_token
// 	// free (content);
// 	//error check if malloc fails in add_token
// }

int	no_quotes_state(char *input, char **res, int *i, t_shell *shell)
{
	int		start;
	char	*content;
	char	*tmp;
	char	*expansion;

	start = (*i);
	while (input[*i] && !ft_strchr(" \"'|><", input[*i]))
		(*i)++;
	if (start == *i)
		return (1);
	content = ft_strndup(&input[start], *i - start);
	if (!content)
		return (0);
	tmp = *res;
	expansion = variable_exp_double(content, shell);
	if (!expansion)
		return (0);
	*res = append_str(tmp, expansion);
	free(tmp);
	if (!*res)
		return (0);
	return (1);
}


int	double_quotes_state(char *input, char **res, int *i, t_shell *shell)
{
	int		start;
	char	*content;
	char	*expansion;

	(*i)++; // move past the first double quote
	start = *i;
	while (input[*i] && input [*i] != '"')
		(*i)++;
	if (input[*i] != '"')
		return (0);
	content = ft_strndup(&input[start], *i - start);
	if (!content)
		return (0);
	expansion = variable_exp_double(content, shell);
	if (!expansion)
	{
		free (content);
		return (0);
	}
	free (content);
	content = *res;
	*res = append_str(content, expansion);
	free (content);
	if (!*res)
		return (0);
	(*i)++; // move past last double quote
	return (1);
}

int	single_quotes_state(char *input, char **res, int *i)
{
	int		start;
	char	*content;
	char	*tmp;

	(*i)++; // move past the first single qoute
	start = *i;
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	if (input[*i] != '\'')
		return (0);
	tmp = *res;
	content = ft_strndup(&input[start], *i - start);
	if (!content)
	{
		free (tmp);
		return (0);
	}
	*res = append_str(tmp, content);
	free(content);
	free (tmp);
	if (!*res)
		return (0);
	(*i)++; // move past the last single quote
	return (1);
}

int	tokenize_word(char *input, int *i, t_token **head, t_shell *shell)
{
	char	*result;

	result = ft_strdup("");
	if (!result)
		return (0);
	while (input[*i] && !is_whitespace(input[*i]) && !is_special_token(input[*i]))
	{
		if (input[*i] == '"')
		{
			if (!double_quotes_state(input, &result, i, shell))
			{
				free (result);
				return (0);
			}
		}
		if (input[*i] == '\'')
		{
			if (!single_quotes_state(input, &result, i))
			{
				free (result);
				return (0);
			}
		}
		else
		{
			if (!no_quotes_state(input, &result, i, shell))
			{
				free (result);
				return (0);
			}
		}
	}
	add_token(head, result, TOKEN_WORD);
	return (1);
}

/* 
if there is no space between execpt if special character, the tokenizer should keep reading? but how to handle if different sorts of types, 
how to handle the characters inside the different types?, expanding before parser? not relying on the tokens? just read until there is a space or a special character? 
different states of how to read the input, depening on if your inside double quotes, single or nothing, even though they are together? 
so just one for all the types handeling it in the tokenizer? and just sending the finished product to the parser as 1 whole word, all expanded and quotes removed?

no quotes:
- special characters: 
	- " "  --> go in to double quote state
	- ' '  --> go in to single quote state
	- >, < --> redirection
	- |    --> pipe
	- $    --> variable expansion

double
- special characters:
	- $ --> expansion

single
- handle everything as literal characters 
*/

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
