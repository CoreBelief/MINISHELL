/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/28 12:19:48 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/08 15:38:43 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "tokenizer.h"

t_token	*add_token(t_token **head, char *content, t_token_type type);
int		tokenize_pipe(int *i, t_token **head);
int		tokenize_redirection(char *input, int *i, t_token **head);
int		tokenize_word(char *input, int *i, t_token **head, t_shell *shell);
int		tokenizer(char *input, t_shell *shell);


t_token	*add_token(t_token **head, char *content, t_token_type type)
{
	t_token	*new_token;
	t_token	*current;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
	{
		free (content);
		content = NULL;
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

int	tokenize_pipe(int *i, t_token **head)
{
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
			if (!handle_quotes_state(input, &result, i, shell)) // exit codes are handled before this point, just returning 0
				return (free (result), 0);
		}
		else
		{
			if (!no_quotes_state(input, &result, i, shell)) // exit codes are handled before this point, just returning 0
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

/* FIXXXXXXXX */
int	tokenizer(char *input, t_shell *shell)
{
	// t_token	*head;
	int		i;
	int		len;

	// head = NULL;
	i = 0;
	len = ft_strlen(input);
	while (input[i] != '\0' && i < len)
	{
		while (is_whitespace(input[i]))
			i++;
		if (input[i] == '|')
		{
			if ((input[i - 1] && input[i - 1] == '|') || !input[i + 1] || i == 0)
			{
				free_tokens(&shell->tokens);
				return (handle_syn_errors(2, "syntax error near unexpected token `|'\n", shell), 0);
			}
			if (!tokenize_pipe(&i, &shell->tokens)) // always malloc fail if returns 0
			{
				free_tokens(&shell->tokens);
				return (handle_syn_errors(1, "malloc fail\n", shell), 0);
			}
		}
		else if (input[i] == '<' || input[i] == '>')
		{
			if (!tokenize_redirection(input, &i, &shell->tokens)) // always malloc fail if returns 0
			{
				free_tokens(&shell->tokens);
				return (handle_syn_errors(1, "malloc fail\n", shell), 0);
			}
		}
		else
		{
			if (!tokenize_word(input, &i, &shell->tokens, shell))
			{
				free_tokens(&shell->tokens);
				return (0);
			}
		}
	}
	if (!shell->tokens)
		shell->last_exit_status = 1;
	// shell->tokens = head;
	return (1);
}
