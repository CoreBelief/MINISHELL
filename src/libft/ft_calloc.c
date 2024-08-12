/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eeklund <eeklund@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 14:55:54 by eeklund           #+#    #+#             */
/*   Updated: 2023/10/18 15:14:22 by eeklund          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*allocated;
	size_t	tot_size;

	tot_size = count * size;
	if (tot_size == 0)
		tot_size = 1;
	else if ((tot_size / size) != count)
		return (NULL);
	allocated = (char *)malloc(tot_size);
	if (!allocated)
		return (NULL);
	ft_bzero(allocated, tot_size);
	return (allocated);
}

/*
int main(void)
{
	//printf("%s", (char *)ft_calloc(2, 0));
	printf("%s", (char *)ft_calloc(INT_MAX, INT_MAX));
}
*/