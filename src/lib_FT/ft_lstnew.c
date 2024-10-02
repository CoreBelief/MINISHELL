/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elleneklund <elleneklund@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 13:38:54 by eeklund           #+#    #+#             */
/*   Updated: 2023/10/18 12:40:20 by elleneklund      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*temp;

	temp = (t_list *)malloc (sizeof(t_list));
	if (!temp)
		return (NULL);
	temp->content = content;
	temp->next = NULL;
	return (temp);
}

// #include <stdio.h>

// void	printnode(struct s_list *node)
// {
// 	if (node == NULL)
// 		return ;
// 	else
// 	{
// 		printf("\ncontent:%s\naddress:%p\nnext:%p\n\n",
//(char*)node->content, node, node->next);
// 	}
// }

// int main(void)
// {
// 	printnode(ft_listnew("hello"));
// 	free(ft_listnew("hello"));
// }