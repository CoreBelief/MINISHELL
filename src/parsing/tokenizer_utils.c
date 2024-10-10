/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer_utils.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/30 17:22:38 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/10 15:39:36 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_whitespace(char c);
bool	is_special_token(char c);
int		handle_quotes_state(char *input, char **res, int *i, t_shell *shell);
int		prev_token_hd(t_token *tok);
t_token	*add_token(t_token **head, char *content, t_token_type type);
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

int	handle_quotes_state(char *input, char **res, int *i, t_shell *shell)
{
	if (input[*i] == '"' )
	{
		if (!double_quotes_state(input, res, i, shell))
			return (0);
	}
	if (input[*i] == '\'')
	{
		if (!single_quotes_state(input, res, i, shell))
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
// void	print_token_list(t_token *head)
// {
// 	t_token	*temp;

// 	printf("inside print token_lst\n");
// 	while (head)
// 	{
// 		temp = head;
// 		head = head->next;
// 		printf("TOKEN-> '%s' type:%i\n", temp->content, temp->type);
// 	}
// 	printf("\n");
// }
