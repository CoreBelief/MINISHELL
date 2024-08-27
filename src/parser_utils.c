/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_utils.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/25 15:36:55 by elleneklund   #+#    #+#                 */
/*   Updated: 2024/08/27 14:11:06 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	is_redirect_token(int token_type)
{
	return (token_type == TOKEN_REDIRECT_IN || \
		token_type == TOKEN_REDIRECT_OUT || \
		token_type == TOKEN_REDIRECT_APPEND || \
		token_type == TOKEN_HEREDOC);
}

int	handle_redirection_parsing(t_command *cmd, t_token **token)
{
	cmd->redir[cmd->redirect_count].type = (*token)->type;
	(*token) = (*token)->next;
	if (token && (*token)->type == TOKEN_WORD)
	{
		cmd->redir[cmd->redirect_count].file = ft_strdup((*token)->content);
		if (!cmd->redir[cmd->redirect_count].file)
			return (0);
		cmd->redirect_count++;
		return (1);
	}
	return (0);
}

int	handle_arg_parsing_2nd(t_command *cmd, t_token **tokens, int *i)
{
	if ((*tokens)->type == TOKEN_DOUBLE_QUOTE)
		variable_exp_double(*tokens, (*tokens)->content);
	else if ((*tokens)->content[0] == '$')
		variable_exp_dollar((*tokens), (*tokens)->content);
	cmd->argv[*i] = ft_strdup((*tokens)->content);
	if (!cmd->argv[*i])
		return (0);
	(*i)++;
	return (1);
}

// int	handle_arg_parsing(t_command *cmd, t_token *tokens, int *i)
// {
// 	if (tokens->type == TOKEN_DOUBLE_QUOTE)
// 		variable_exp_double(tokens, tokens->content);
// 	else if (tokens->content[0] == '$')
// 		variable_exp_dollar(tokens, tokens->content);
// 	cmd->argv[*i] = ft_strdup(tokens->content);
// 	if (!cmd->argv[*i])
// 		return (0);
// 	(*i)++;
// 	return (1);
// }

t_command	*handle_pipe_parsing(t_command *cmd, int *i)
{
	t_command	*new_cmd;

	new_cmd = init_cmd();
	if (!new_cmd)
		return (NULL); // Handle error
	cmd->argv[*i] = NULL;
	cmd->pipe_out = PIPE_OUT; // 
	cmd->next = new_cmd;
	if (new_cmd)
		new_cmd->pipe_in = PIPE_IN;
	*i = 0;
	return (new_cmd);
}

void	set_command_paths(t_command *cur_cmd)
{
	while (cur_cmd)
	{
		cur_cmd->path = ft_strdup(cur_cmd->argv[0]);
		if (!cur_cmd->path)
			return ; // Handle error
		cur_cmd = cur_cmd->next;
	}
}



//DEUG FUNCTION
void	print_cmd_list(t_command *head)
{
	// t_command	*temp;
	int			i;
	int			j;

	printf("inside print cmd_lst\n");
	// temp = head;
	while (head)
	{
		printf("command path-> '%s' redir count:%i\n", head->path, head->redirect_count);
		i = 0;
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