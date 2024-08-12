/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eeklund <eeklund@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 12:40:43 by elleneklund       #+#    #+#             */
/*   Updated: 2023/10/24 18:15:24 by eeklund          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

//#include <stdio.h>
// int main(void)
// {
// 	t_list *head = ft_lstnew("Node 1");
//     t_list *node2 = ft_lstnew("Node 2");
//     t_list *node3 = ft_lstnew("Node 3");
//     head->next = node2;
//     node2->next = node3;
// 	ft_lstlast(head);
// }
//printf("THE last NODE:\ncontent:%s\naddress:%p\nnext:%p\n\n", /
//(char*)lst->content, lst, lst->next);
//https://www.csnp.org/post/capture-the-flag-for-beginners 
//capture the flag hacker style :O