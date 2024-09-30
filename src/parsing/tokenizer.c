/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/28 12:19:48 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/30 19:06:02 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "tokenizer.h"

int		tokenize_pipe(int *i, t_token **head);
void	tokenize_redirection(char *input, int *i, t_token **head);
t_token	*tokenizer(char *input, t_shell *shell);

int	tokenize_pipe(int *i, t_token **head)
{
	if (*i == 0)
	{
		printf("syntax error near unexpected token `|'\n");
		return (0); //here we should return an error code (2)?
	}
	if (!add_token(head, ft_strdup("|"), TOKEN_PIPE))
		return (0);
	(*i)++;
	return (1);
}

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
