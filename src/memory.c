#include "minishell.h"
#include <stdlib.h>

void	free_command(t_cmd *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->argv)
	{
		i = 0;
		while (cmd->argv[i])
		{
			free(cmd->argv[i]);
			cmd->argv[i] = NULL;
			i++;
		}
		free(cmd->argv);
		cmd->argv = NULL;
	}
	if (cmd->redir)
	{
		i = 0;
		while (i < cmd->redirect_count)
		{
			free(cmd->redir[i].file);
			cmd->redir[i].file = NULL;
			i++;
		}
		free(cmd->redir);
		cmd->redir = NULL;
	}
	free(cmd);
}

void	free_command_list(t_cmd **head)
{
	t_cmd	*tmp;

	if (!head || !*head)
		return ;
	while (*head)
	{
		tmp = *head;
		*head = (*head)->next;
		free_command(tmp);
	}
	*head = NULL;
}

void	ft_free_str_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free (arr[i]);
		i++;
	}
	free (arr);
}


void	free_tokens(t_token **head)
{
	t_token	*temp;

	if (!head)
		return ;
	while (*head)
	{
		temp = *head;
		*head = (*head)->next;
		if (temp->content)
		{
			free(temp->content);
			temp->content = NULL;
		}
		free(temp);
	}
	*head = NULL;
}
