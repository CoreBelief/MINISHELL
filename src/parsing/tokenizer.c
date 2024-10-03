/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/28 12:19:48 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/03 13:41:00 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "tokenizer.h"

int		tokenize_pipe(int *i, t_token **head);
int		tokenize_redirection(char *input, int *i, t_token **head);
t_token	*tokenizer(char *input, t_shell *shell);

int	tokenize_pipe(int *i, t_token **head)
{
	// if (*i == 0)
	// {
	// 	printf("syntax error near unexpected token `|'\n");
	// 	return (0); //here we should return an error code (2)?
	// }
	if (!add_token(head, ft_strdup("|"), TOKEN_PIPE))
		return (0);
	(*i)++;
	return (1);
}

int	tokenize_redirection(char *input, int *i, t_token **head)
{
	t_token	*tmp;

	if (input[*i + 1] == input[*i])
	{
		if (input[*i] == '>')
			tmp = add_token(head, ft_strdup(">>"), TOKEN_REDIRECT_APPEND);
		else
			tmp = add_token(head, ft_strdup("<<"), TOKEN_HEREDOC);
		(*i) = (*i) + 2;
	}
	else
	{
		if (input[*i] == '>')
			tmp = add_token(head, ft_strdup(">"), TOKEN_REDIRECT_OUT);
		else
			tmp = add_token(head, ft_strdup("<"), TOKEN_REDIRECT_IN);
		(*i)++;
	}
	if (!tmp)
		return (0);
	return (1);
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
		// if (input[i] == '\0')
		// 	break ;
		if (input[i] == '|')
		{
			if ((input[i - 1] && input[i - 1] == '|') || !input[i + 1] || i == 0)
				return (handle_syn_errors(2, "syntax error near unexpected token `|'\n", shell));
			if (!tokenize_pipe(&i, &head))
				break ;
		}
		else if (input[i] == '<' || input[i] == '>')
		{
			if (!tokenize_redirection(input, &i, &head))
				return (handle_syn_errors(1, "malloc fail\n", shell));
		}
		else
		{
			if (!tokenize_word(input, &i, &head, shell))
				return (handle_syn_errors(1, "malloc fail\n", shell));
		}
	}
	if (!head)
		shell->last_exit_status = 1;
	return (head);
}
