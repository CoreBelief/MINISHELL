/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/28 12:19:48 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/10 15:38:32 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "tokenizer.h"

int		tokenize_pipe(char *input, int *i, t_shell *shell);
int		tokenize_redirection(char *input, int *i, t_token **head);
int		tokenize_word(char *input, int *i, t_token **head, t_shell *shell);
int		tokenize_all(char *input, int *i, t_shell *shell);
int		tokenizer(char *input, t_shell *shell);

int	tokenize_pipe(char *input, int *i, t_shell *shell)
{
	if (*i == 0 || !input[*i + 1] || (input[*i + 1] && input[*i + 1] == '|'))
	{
		free_tokens(&shell->tokens);
		return (handle_syn_errors(2, \
		"syntax error near unexpected token `|'\n", shell), 0);
	}
	if (!add_token(&shell->tokens, ft_strdup("|"), TOKEN_PIPE))
	{
		free_tokens(&shell->tokens);
		return (handle_syn_errors(1, "malloc fail\n", shell), 0);
	}
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

int	tokenize_word(char *input, int *i, t_token **head, t_shell *shell)
{
	char	*result;

	result = ft_strdup("");
	if (!result)
		return (0);
	while (input[*i] && !is_whitespace(input[*i]) && \
	!is_special_token(input[*i]))
	{
		if (input[*i] == '"' || input[*i] == '\'')
		{
			if (!handle_quotes_state(input, &result, i, shell))
				return (free (result), 0);
		}
		else
		{
			if (!no_quotes_state(input, &result, i, shell))
				return (free (result), 0);
		}
	}
	if (!add_token(head, result, TOKEN_WORD))
	{
		free (result);
		return (handle_syn_errors(1, "Malloc fail\n", shell), 0);
	}
	return (1);
}

int	tokenize_all(char *input, int *i, t_shell *shell)
{
	if (input[*i] == '|')
	{
		if (!tokenize_pipe(input, i, shell))
			return (0);
	}
	else if (input[*i] == '<' || input[*i] == '>')
	{
		if (!tokenize_redirection(input, i, &shell->tokens))
		{
			free_tokens(&shell->tokens);
			return (handle_syn_errors(1, "malloc fail\n", shell), 0);
		}
	}
	else
	{
		if (!tokenize_word(input, i, &shell->tokens, shell))
			return (free_tokens(&shell->tokens), 0);
	}
	return (1);
}

int	tokenizer(char *input, t_shell *shell)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(input);
	while (input[i] != '\0' && i < len)
	{
		while (is_whitespace(input[i]))
			i++;
		if (!tokenize_all(input, &i, shell))
			return (0);
	}
	if (!shell->tokens)
	{
		shell->last_exit_status = 1;
		return (0);
	}
	return (1);
}
