/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elleneklund <elleneklund@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 10:45:31 by elleneklund       #+#    #+#             */
/*   Updated: 2023/10/18 12:37:26 by elleneklund      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*trimmedstring;
	size_t	front;
	size_t	back;

	front = 0;
	back = ft_strlen((char *)s1);
	while (s1[front] && ft_strchr(set, s1[front]))
		front++;
	while (back > front && ft_strchr(set, s1[back - 1]))
		back--;
	trimmedstring = (char *)malloc(sizeof(char) * (back - front) + 1);
	if (!trimmedstring)
		return (NULL);
	ft_strlcpy(trimmedstring, &s1[front], (back - front) + 1);
	return (trimmedstring);
}

/*int	main(void)
{
	char *trimmed = ft_strtrim("eeeeeejokejbraeeeeehhhheh", "eh");
	printf("%s",trimmed);
	free(trimmed);
}*/
