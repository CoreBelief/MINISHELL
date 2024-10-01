/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eeklund <eeklund@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 17:05:09 by eeklund           #+#    #+#             */
/*   Updated: 2023/10/17 14:13:34 by eeklund          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	*str;

	str = (char *)s;
	i = ft_strlen(str) - 1;
	if (c == '\0')
		return (&str[i + 1]);
	while (i >= 0)
	{
		if (str[i] == (char)c)
			return (&str[i]);
		i--;
	}
	return (NULL);
}

// #include <stdio.h>

// int main(void)
// {
// 	printf("%s", ft_strrchr("hje po dig", '\0'));
// 	printf("%s", ft_strrchr("hje po dig", 'h'));
// 	printf("%s", ft_strrchr("hje po dig", 'h' + 256));
// }