/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstadd_back.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/15 12:43:06 by elleneklund   #+#    #+#                 */
/*   Updated: 2024/10/02 19:20:23 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*tmp;

	if (new && lst)
	{
		if (!*lst)
			*lst = new;
		else
		{
			tmp = ft_lstlast(*lst);
			tmp->next = new;
		}
	}
}

// void	ft_lstadd_back(t_list **lst, t_list *new)
// {
// 	t_list	*temp;

// 	temp = *lst;
// 	if (temp == NULL)
// 	{
// 		*lst = new;
// 	}
// 	else
// 	{
// 		while ((temp -> next) != NULL)
// 		{
// 			temp = temp->next;
// 		}
// 		temp -> next = new;
// 	}
// }

// #include <stdio.h>

// int main(void)
// {
// }
