/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eeklund <eeklund@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 15:10:58 by eeklund           #+#    #+#             */
/*   Updated: 2023/10/13 13:25:11 by eeklund          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char	*dst;
	char	*source;
	size_t	i;

	dst = (char *)dest;
	source = (char *)src;
	i = 0;
	while (i < n)
	{
		dst[i] = source[i];
		i++;
	}
	return (dest);
}

// The  memcpy() function copies n bytes from memory area src
//	to memory area dest.The memory areas must not overlap.
//	Use memmove(3) if the memory areas do overlap.
// int    main(void)
// {
// 	char   destination[10];
// 	//puts(ft_memcpy(destination, "heeeej", 7));
// 		char str[10] = "hejola";

// 	ft_memcpy(&str[0], &str[1], 4);
// 	printf("%s", str);

// 	char dest[100];
// 	memset(dest, 'A', 100);
// 	ft_memcpy(dest, "coucou", 0);
// 	memcpy(dest, "coucou", 0);
// 	printf("%c\n", dest[0]); // = 'A'
// 	char *rtn = (char *)ft_memcpy(dest, NULL, 0);
// 	char *rtn = (char *)memcpy(dest, NULL, 0);
// 	printf("rtn: %s\n", rtn);
// 	printf("dest: %s\n", dest);
// 	printf("%c\n", dest[0]); // = 'A'
// 	char src[] = {0, 0};
// 	ft_memcpy(dest, src, 2);
// 	int i = 0;
// 	for (; i < 100 && dest[i] == 0; ++i)
// 		;
// 	printf("%c\n", dest[2]); // == 'A'
// 	return (0);
// }
