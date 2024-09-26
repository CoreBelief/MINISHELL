/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_utils.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/25 15:36:55 by elleneklund   #+#    #+#                 */
/*   Updated: 2024/09/26 19:01:04 by rdl           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	remove_quotes(char *str)
{
	int		i;
	int		j;
	int		in_quotes = 0;
	char	quote_char = 0;

	i = 0;
	j = 0;
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

int	handle_arg_parsing_2nd(t_cmd *cmd, t_token **tokens, int *i, t_shell *shell)
{
	char	*tmp;

	tmp = (*tokens)->content;
	if ((*tokens)->type == TOKEN_DOUBLE_QUOTE || (*tokens)->type != TOKEN_SINGLE_QUOTE)
	{
		if (!variable_exp_double(*tokens, tmp, shell))
			return (0);
		free (tmp);
	}
	if ((*tokens)->content[0] == '\0') // ducktape solution to having empty expansions in the cmd line, gives wrong exitcode if only $nonexistent
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
	int			i = 0;
	int			j;

	printf("inside print cmd_lst\n");
	while (head)
	{
		printf("redir count:%i\n",head->redirect_count);
		while (head->argv && head->argv[i])
		{
			printf("arg: %s\n", head->argv[i]);
			i++;
		}
		if (head->redirect_count != 0)
		{
			printf("redirecttype == %i target file == %s\n", head->redir->type, head->redir->file);
		}
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
