#include "minishell.h"
#include <stdlib.h>

void	free_command(t_command *cmd)
{
	int	i;

	if (!cmd)
		return;
	if (cmd->path)
		free(cmd->path);
	if (cmd->argv)
	{
		i = 0;
		while (cmd->argv[i])
		{
			free(cmd->argv[i]);
			i++;
		}
		free(cmd->argv);
	}
	if (cmd->redir)
	{
		i = 0;
		while (i < cmd->redirect_count)
		{
			free(cmd->redir[i].file);
			i++;
		}
		free(cmd->redir);
	}
	free(cmd);
}

void	free_command_list(t_command **head)
{
	t_command	*tmp;

	if (!head || !*head)
		return ;
	while (*head)
	{
		tmp = *head;
		free_command(tmp);
		*head = (*head)->next;
	}
}



void	ft_free_str_array(char **arr)
{
	int	i;

	if (!arr)
		return;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);  
		i++;
	}
	free(arr);  
}