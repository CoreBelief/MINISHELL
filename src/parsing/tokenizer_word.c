/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer_word.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/30 18:42:40 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/10 15:40:24 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	extract_expansion(char *content, char **res, t_shell *shell);
int	extract_and_append(char *input, int len, char **res, t_shell *shell);
int	double_quotes_state(char *input, char **res, int *i, t_shell *shell);
int	single_quotes_state(char *input, char **res, int *i, t_shell *shell);
int	no_quotes_state(char *input, char **res, int *i, t_shell *shell);

int	extract_expansion(char *content, char **res, t_shell *shell)
{
	char	*expansion;
	char	*tmp;

	tmp = *res;
	expansion = variable_exp_double(content, shell);
	free(content);
	if (!expansion)
		return (0);
	*res = append_str(tmp, expansion);
	free(tmp);
	free(expansion);
	return (*res != NULL);
}

int	extract_and_append(char *input, int len, char **res, t_shell *shell)
{
	char	*content;
	char	*tmp;

	content = ft_strndup(input, len);
	if (!content)
		return (0);
	if (ft_strchr(content, '$') && !prev_token_hd(shell->tokens))
	{
		if (!extract_expansion(content, res, shell))
			return (0);
	}
	else
	{
		tmp = *res;
		*res = append_str(tmp, content);
		free(tmp);
		free(content);
	}
	if (!*res)
		return (0);
	return (1);
}

int	double_quotes_state(char *input, char **res, int *i, t_shell *shell)
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
		return (handle_syn_errors(1, "Malloc fail\n", shell), 0);
	(*i)++;
	return (1);
}

int	single_quotes_state(char *input, char **res, int *i, t_shell *shell)
{
	int		start;
	char	*content;
	char	*tmp;

	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	if (input[*i] != '\'')
		return (handle_syn_errors(2, "syntax error\n", shell), 0);
	tmp = *res;
	content = ft_strndup(&input[start], *i - start);
	if (!content)
	{
		free (tmp);
		return (handle_syn_errors(1, "Malloc fail\n", shell), 0);
	}
	*res = append_str(tmp, content);
	free(content);
	free (tmp);
	if (!*res)
		return (handle_syn_errors(1, "Malloc fail\n", shell), 0);
	(*i)++;
	return (1);
}

int	no_quotes_state(char *input, char **res, int *i, t_shell *shell)
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
	if (!extract_and_append(&input[start], *i - start, res, shell))
		return (handle_syn_errors(1, "Malloc fail\n", shell), 0);
	return (1);
}
