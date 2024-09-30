/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer_utils.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/30 17:22:38 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/30 17:20:30 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"
#include "minishell.h"
void	tokenize_redirection(char *input, int *i, t_token **head);
bool	is_special_token(char c);
int	no_quotes_state(char *input, char **res, int *i, t_shell *shell);
int	double_quotes_state(char *input, char **res, int *i, t_shell *shell);
int	single_quotes_state(char *input, char **res, int *i);
int	tokenize_word(char *input, int *i, t_token **head, t_shell *shell);
void	print_token_list(t_token *head);


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
	free (content);
	if (!expansion)
		return (0);
	*res = append_str(tmp, expansion);
	free(tmp);
	free (expansion);
	if (!*res)
		return (0);
	return (1);
}


int	double_quotes_state(char *input, char **res, int *i, t_shell *shell)
{	//function too long
	int		start;
	char	*content;
	char	*expansion;

	(*i)++;
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
	free (expansion);
	if (!*res)
		return (0);
	(*i)++;
	return (1);
}

int	single_quotes_state(char *input, char **res, int *i)
{
	int		start;
	char	*content;
	char	*tmp;

	(*i)++;
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
	(*i)++;
	return (1);
}

int	tokenize_word(char *input, int *i, t_token **head, t_shell *shell)
{	//function too long
	char	*result;

	result = ft_strdup("");
	if (!result)
		return (0);
	while (input[*i] && !is_whitespace(input[*i]) && \
	!is_special_token(input[*i]))
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
	if (!add_token(head, result, TOKEN_WORD))
	{
		free (result);
		return (0);
	}
	return (1);
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
