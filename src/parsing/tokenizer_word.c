/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer_word.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/30 18:42:40 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/02 17:08:02 by rdl           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	extract_and_append(char *input, int len, char **res, t_shell *shell);
int	no_quotes_state(char *input, char **res, int *i, t_shell *shell);
int	double_quotes_state(char *input, char **res, int *i, t_shell *shell);
int	single_quotes_state(char *input, char **res, int *i);

int	extract_and_append(char *input, int len, char **res, t_shell *shell)
{
	char	*content;
	char	*expansion;
	char	*tmp;

	content = ft_strndup(input, len);
	if (!content)
		return (0);
	expansion = variable_exp_double(content, shell);
	free(content);
	if (!expansion)
		return (0);
	tmp = *res;
	*res = append_str(tmp, expansion);
	free(tmp);
	free(expansion);
	return (*res != NULL);
}

int	no_quotes_state(char *input, char **res, int *i, t_shell *shell)
{
	int		start;

	start = (*i);
	while (input[*i] && !ft_strchr(" \"'|><", input[*i]))
		(*i)++;
	if (start == *i)
		return (1);
	return (extract_and_append(&input[start], *i - start, res, shell));
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
		printf("syntax error\n");
		shell->last_exit_status = 2;
		return (0);
	}
	if (!extract_and_append(&input[start], *i - start, res, shell))
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
	{
		printf("syntax error\n");
		return (0);
	}
	tmp = *res;
	content = ft_strndup(&input[start], *i - start);
	if (!content)
		return (free (tmp), 0);
	*res = append_str(tmp, content);
	free(content);
	free (tmp);
	if (!*res)
		return (0);
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
		return (free (result), 0);
	return (1);
}
