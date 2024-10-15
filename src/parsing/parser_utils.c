/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_utils.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/25 15:36:55 by elleneklund   #+#    #+#                 */
/*   Updated: 2024/10/10 15:45:40 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int		handle_word_parsing(t_cmd *cmd, t_token **tokens, int *i);
t_cmd	*handle_pipe_parsing(t_cmd *cmd, int *i);

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
