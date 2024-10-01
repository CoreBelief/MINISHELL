/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eeklund <eeklund@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 20:26:50 by eeklund           #+#    #+#             */
/*   Updated: 2023/10/18 16:08:56 by eeklund          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void*), void (*del)(void *))
{
	t_list	*tmp;
	t_list	*newhead;

	if (!lst || !f || !del)
		return (NULL);
	newhead = NULL;
	while (lst)
	{
		tmp = lst;
		tmp = ft_lstnew(f(tmp->content));
		if (!tmp)
		{
			ft_lstclear(&newhead, del);
			return (NULL);
		}
		ft_lstadd_back(&newhead, tmp);
		lst = lst->next;
	}
	return (newhead);
}

// #include <stdio.h>

// void printList(t_list *head) {
//     t_list *current = head;

//     while (current) {
//         printf("%d ", *(int *)current->content);
//         current = current->next;
//     }
//     printf("\n");
// }

// void freeList(t_list *head)
// {
// 	if (head) freeList((t_list *)head->next); free(head);
// }

// void * addOne(void * p) 
// {
// 	void * r = malloc(sizeof(int)); *(int*)r = *(int*)p + 1; return (r);
// }

// int main(void)
// {
// 	int tab[] = {0, 1, 2, 3};
// 	t_list * l =  ft_lstnew(tab);
// 	for (int i = 1; i < 4; ++i)
// 		ft_lstadd_back(&l, ft_lstnew(tab + i));
// 	printf("Original List: ");
// 	printList(l);
// 	t_list * m = ft_lstmap(l, addOne, free);
// 	printf("Modified List: ");
// 	printList(m);
// 	freeList(l);
// 	freeList(m);
// }