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

// void	free_command_list(t_cmd **head)
// {
// 	t_cmd	*tmp;

// 	if (!head || !*head)
// 		return ;
// 	while (*head)
// 	{
// 		tmp = *head;
// 		free_command(tmp);
// 		*head = (*head)->next;
// 	}	
// }
void	free_command_list(t_cmd **head)
{ //does this fix the vallgrind error??
	t_cmd	*tmp;
	t_cmd	*next_node;

	if (!head || !*head)
		return ;
	while (*head)
	{
		tmp = *head;
		next_node = tmp->next;   // Store the next node before freeing
		free_command(tmp);
		*head = next_node;       // Move head to the next node
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