/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eeklund <eeklund@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 10:19:36 by elleneklund       #+#    #+#             */
/*   Updated: 2023/10/19 09:56:51 by eeklund          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substring;
	size_t	i;
	size_t	slen;

	if (!s)
		return (NULL);
	slen = ft_strlen(s);
	if (start >= slen)
		len = 0;
	if (len > slen - start)
		len = slen - start;
	substring = (char *)malloc(sizeof(char) * (len + 1));
	if (!substring)
		return (NULL);
	i = 0;
	while (i < len && s[start + i])
	{
		substring[i] = s[start + i];
		i++;
	}
	substring[i] = '\0';
	return (substring);
}

//  int main(void)
//  {
// //     // char string[10] = "hellooo";
// //     // char *sub = ft_substr(string, 3, 3);
// //     // puts(sub);
// //     // free (sub);
// 	char * s = ft_substr("tripouille", 0, 42000);
// // 	/* 2 */ printf("s: %s\n", s);
// // 	printf("strlen test: %lu\n", strlen("tripouille") + 1);
// 	s = ft_substr("tripouille", 1, 1);
// 	s = ft_substr("tripouille", 100, 1);
// 	/* 3 */ if (!strcmp(s, ""))
// 				printf("ok!");
// 			else
// 				printf("substring:%s", s);
//  	free(s); 
//  }