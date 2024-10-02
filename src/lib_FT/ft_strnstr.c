/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eeklund <eeklund@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 17:54:04 by eeklund           #+#    #+#             */
/*   Updated: 2023/10/19 09:51:28 by eeklund          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	int		lenl;

	i = 0;
	lenl = ft_strlen(little);
	if (lenl == 0)
		return ((char *)big);
	while (big[i] && i < len)
	{
		if (big[i] != *little)
			i++;
		else
		{
			if (!(ft_strncmp(&big[i], little, lenl)) && i + lenl <= len)
				return ((char *)&big[i]);
			i++;
		}
	}
	return (NULL);
}

// #include <stdio.h>

// int main(void)
// {
// 	printf("%s" ,ft_strnstr("hejokej", "ok", 4));
// }