/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer_word.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/30 18:42:40 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/04 20:11:13 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	extract_and_append(char *input, int len, char **res, t_shell *shell);
int	no_quotes_state(char *input, char **res, int *i, t_shell *shell);
int	double_quotes_state(char *input, char **res, int *i, t_shell *shell);
int	single_quotes_state(char *input, char **res, int *i, t_shell *shell);

int	extract_and_append(char *input, int len, char **res, t_shell *shell) // needs to know if the previous token is a heredoc --> not do expansion just keep unchanged
{
	char	*content;
	char	*expansion;
	char	*tmp;

	content = ft_strndup(input, len);
	if (!content)
		return (handle_syn_errors(2, "Malloc fail\n", shell), 0);
	tmp = *res;
	if (ft_strchr(content, '$'))
	{
		expansion = variable_exp_double(content, shell);
		free(content);
		if (!expansion)
			return (handle_syn_errors(2, "Malloc fail\n", shell), 0);
		*res = append_str(tmp, expansion);
		free(tmp);
		free(expansion);
	}
	else
	{
		*res = append_str(tmp, content);
		free(tmp);
		free(content);
	}
	if (!*res)
		return (handle_syn_errors(2, "Malloc fail\n", shell), 0);
	return (1);
}

int	no_quotes_state(char *input, char **res, int *i, t_shell *shell) // exit codes handled before this point
{
	int		start;

	start = (*i);
	while (input[*i] && !ft_strchr(" \"'|><", input[*i]))
	{
		if (input[*i] == '$' && input [*i + 1] == '"')
		{
			(*i)++;
			return (double_quotes_state(input, res, i, shell));
		}
		else if (input[*i] == '$' && input [*i + 1] == '\'')
		{
			(*i)++;
			return (single_quotes_state(input, res, i, shell));
		}
		(*i)++;
	}
	if (start == *i)
		return (1);
	return (extract_and_append(&input[start], *i - start, res, shell));
}

int	double_quotes_state(char *input, char **res, int *i, t_shell *shell) // handle exit codes here
{
	int	start;

	(*i)++;
	start = *i;
	while (input[*i] && input [*i] != '"')
		(*i)++;
	if (input[*i] != '"')
	{
		return (handle_syn_errors(2, "syntax error\n", shell), 0);
	}
	if (!extract_and_append(&input[start], *i - start, res, shell))
		return (handle_syn_errors(2, "Malloc fail\n", shell), 0);
	(*i)++;
	return (1);
}

int	single_quotes_state(char *input, char **res, int *i, t_shell *shell) // handle exit codes here
{
	int		start;
	char	*content;
	char	*tmp;

	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	if (input[*i] != '\'')
	{
		return (handle_syn_errors(2, "syntax error\n", shell), 0);
	}
	tmp = *res;
	content = ft_strndup(&input[start], *i - start);
	if (!content)
	{
		free (tmp);
		return (handle_syn_errors(2, "Malloc fail\n", shell), 0);
	}
	*res = append_str(tmp, content);
	free(content);
	free (tmp);
	if (!*res)
		return (handle_syn_errors(2, "Malloc fail\n", shell), 0);
	(*i)++;
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
		return (handle_syn_errors(2, "Malloc fail\n", shell), 0);
	}
	return (1);
}
