/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_utils.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/25 15:36:55 by elleneklund   #+#    #+#                 */
/*   Updated: 2024/09/30 16:15:43 by rdl           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
void	remove_quotes(char *str);
int	handle_word_parsing(t_cmd *cmd, t_token **tokens, int *i);
t_cmd	*handle_pipe_parsing(t_cmd *cmd, int *i);
void	print_cmd_list(t_cmd *head);

void	remove_quotes(char *str)
{ // function too long
	int		i;
	int		j;
	int		in_quotes;
	char	quote_char;

	i = 0;
	j = 0;
	in_quotes = 0;
	quote_char = 0;
	if (!str)
		return ;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '\"') && \
		(!in_quotes || str[i] == quote_char))
		{
			in_quotes = !in_quotes;
			if (in_quotes)
				quote_char = str[i];
			else
				quote_char = 0;
		}
		else
			str[j++] = str[i];
		i++;
	}
	str[j] = '\0';
}

int	handle_word_parsing(t_cmd *cmd, t_token **tokens, int *i)
{
	if ((*tokens)->content[0] == '\0') 
		return (1);
	cmd->argv[*i] = ft_strdup((*tokens)->content);
	if (!cmd->argv[*i])
		return (0);
	(*i)++;
	return (1);
}

t_cmd	*handle_pipe_parsing(t_cmd *cmd, int *i)
{
	t_cmd	*new_cmd;

	new_cmd = init_cmd();
	if (!new_cmd)
		return (NULL);
	cmd->argv[*i] = NULL;
	cmd->pipe_out = 1;
	cmd->next = new_cmd;
	new_cmd->pipe_in = 1;
	*i = 0;
	return (new_cmd);
}

void	print_cmd_list(t_cmd *head)
{
	int			i;
	int			j;

	printf("inside print cmd_lst\n");
	i = 0;
	while (head)
	{
		printf("redir count:%i\n", head->redirect_count);
		while (head->argv && head->argv[i])
		{
			printf("arg: %s\n", head->argv[i]);
			i++;
		}
		if (head->redirect_count != 0)
			printf("redirecttype == %i target file == %s\n", head->redir->type, head->redir->file);
		printf("  Pipe In: %d, Pipe Out: %d\n", head->pipe_in, head->pipe_out);
		j = 0;
		for (j = 0; j < head->redirect_count; j++)
		{
			printf("  Redirect[%d] Type: %d, File: %s\n", j, head->redir[j].type, head->redir[j].file);
		}
		head = head->next;
	}
	printf("\n");
}
