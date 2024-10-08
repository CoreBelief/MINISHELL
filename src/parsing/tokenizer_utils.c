/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer_utils.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/30 17:22:38 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/08 15:12:19 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_whitespace(char c);
bool	is_special_token(char c);
int		handle_quotes_state(char *input, char **res, int *i, t_shell *shell);
int		prev_token_hd(t_token *tok);
void	print_token_list(t_token *head);

int	is_whitespace(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

bool	is_special_token(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	handle_quotes_state(char *input, char **res, int *i, t_shell *shell) // exit codes handled before this
{
	if (input[*i] == '"' )
	{
		if (!double_quotes_state(input, res, i, shell)) // can be either malloc or syntx fail
			return (0);
	}
	if (input[*i] == '\'')
	{
		if (!single_quotes_state(input, res, i, shell)) // can be either malloc or syntx fail
			return (0);
	}
	return (1);
}

int	prev_token_hd(t_token *tok)
{
	if (!tok)
		return (0);
	while (tok->next)
		tok = tok->next;
	if (tok->type == TOKEN_HEREDOC)
		return (1);
	return (0);
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
