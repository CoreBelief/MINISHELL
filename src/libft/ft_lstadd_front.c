/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elleneklund <elleneklund@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 14:17:53 by eeklund           #+#    #+#             */
/*   Updated: 2023/10/18 11:57:44 by elleneklund      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!new || !lst)
		return ;
	new->next = (*lst);
	*lst = new;
}

// #include <stdio.h>

// void	printnodelst(struct s_list *node)
// {
//  	if (node == NULL)
//  		return ;
// 	while(node)
// 	{
// 		printf("\ncontent:%s\naddress:%p\nnext:%p\n\n",
// 		 (char*)node->content, node, node->next);
// 		node = node->next;
// 	}
// 	printf("--------\n");
// }

// void printnode(t_list *node)
// {
// 	if (node == NULL)
// 		return ;
// 	else
// 		printf("THE NODE:\ncontent:%s\naddress:%p\nnext:%p\n\n",
// (char*)node->content, node, node->next);
// }

// int main(void)
// {
//  	t_list	*node;
// 	t_list	*node1;
// 	t_list	*node2;
// 	node = ft_lstnew("node 0");
// 	node1 = ft_lstnew("node 1");
// 	node2 = ft_lstnew("node 2");
// 	ft_lstadd_front(&node, node1);
// 	ft_lstadd_front(&node, node2);
// 	printnodelst(node);
// 	free(node);
// }
