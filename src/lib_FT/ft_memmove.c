/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eeklund <eeklund@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 16:23:53 by eeklund           #+#    #+#             */
/*   Updated: 2023/10/13 13:25:28 by eeklund          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char	*dst;
	char	*source;
	size_t	i;

	dst = (char *)dest;
	source = (char *)src;
	if (dst < source)
		ft_memcpy(dest, src, n);
	else
	{
		i = 1;
		while (i <= n)
		{
			dst[n - i] = source[n - i];
			i++;
		}
	}
	return (dest);
}

// int main(void)
// {
// //	char str[25] = "hello world";
// //	
// //	printf("%s", (char*)ft_memmove(&str[6], str, 11));
// 	char s[] = {65, 66, 67, 68, 69, 0, 45};
// 	char s0[] = { 0,  0,  0,  0,  0,  0, 0};
// 	char sCpy[] = {65, 66, 67, 68, 69, 0, 45};
// 	char sResult[] = {67, 68, 67, 68, 69, 0, 45};
// 	char sResult2[] = {67, 67, 68, 68, 69, 0, 45};

// 	printf("%s\n"(ft_memmove(s0, s, 7))); // == s0
// 	// && !memcmp(s, s0, 7)); showLeaks(); //Post 0
// /* 2 */ check(ft_memmove(s, s + 2, 0) && !memcmp(s, sCpy, 7)); 
// /* 3 */ check(ft_memmove(s, s + 2, 2) == s && !memcmp(s, sResult, 7));
// /* 4 */ check(ft_memmove(sResult + 1, sResult, 2) == sResult + 1
// && !memcmp(sResult, sResult2, 7));
// 	return (0);
// }