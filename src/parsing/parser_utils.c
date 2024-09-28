/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_utils.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/25 15:36:55 by elleneklund   #+#    #+#                 */
/*   Updated: 2024/09/28 16:58:04 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	remove_quotes(char *str)
{
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
	if ((*tokens)->content[0] == '\0') // ducktape solution to having empty expansions in the cmd line, gives wrong exitcode if only $nonexistent
		return (1);
	cmd->argv[*i] = ft_strdup((*tokens)->content);
	// free ((*tokens)->content); //is getting freed at a later state in free_tokens()
	// (*tokens)->content = NULL;
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
		return (NULL); // Handle error
	cmd->argv[*i] = NULL;
	cmd->pipe_out = 1;
	cmd->next = new_cmd;
	new_cmd->pipe_in = 1;
	*i = 0;
	// printf("cur_cmd in pipe pars %s\n", (cmd)->argv[0]);
	return (new_cmd);
}

//DEUG FUNCTION
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

// gives seg fault if the input ends with a pipe, it tires to access 
// cur_cmd->argv[0] which doesnt exists
// i think if it ends with a pipe it is waiting for that command from stdin. 
// void	set_cmd_paths(t_cmd *cur_cmd)
// {
// 	while (cur_cmd)
// 	{
// 		// printf("hello1\n");
// 		cur_cmd->path = ft_strdup(cur_cmd->argv[0]);
// 		if (!cur_cmd->path)
// 			return ; // Handle error
// 		cur_cmd = cur_cmd->next;
// 		// printf("hello\n");
// 	}
// }