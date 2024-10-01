/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eeklund <eeklund@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 16:40:55 by eeklund           #+#    #+#             */
/*   Updated: 2023/10/19 09:38:13 by eeklund          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int		i;

	i = 0;
	if (c == '\0')
		return ((char *)&s[ft_strlen(s)]);
	while (s[i])
	{
		if (s[i] == (char)c)
			return ((char *)&s[i]);
		i++;
	}
	return (NULL);
}

//The strchr() function returns a pointer to the
// first occurrence of the character c in the string s.

// int main(void)
// {
// 	char *s = "tripouille";
// 	printf("%s\n", ft_strchr(s, 0)); //== s + strlen(s)); showLeaks();
// 	printf("%s\n", strchr(s, 0));
// 	printf("%s\n", ft_strchr(s, 't' + 256)); // == s); showLeaks();
// 	printf("%s\n", strchr(s, 't' + 256));
// }