/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eeklund <eeklund@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 17:06:47 by eeklund           #+#    #+#             */
/*   Updated: 2023/10/19 09:50:13 by eeklund          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// char transform_char(unsigned int index, char c) 
// {
//     return c + index;
// }

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		i;
	char	*res;
	int		len;

	if (!s || !f)
		return (0);
	len = ft_strlen(s);
	res = (char *)malloc((len + 1) * sizeof(char));
	if (!res)
		return (NULL);
	i = 0;
	while (s[i])
	{
		res[i] = f(i, s[i]);
		i++;
	}
	res[i] = '\0';
	return (res);
}

//#include <stdio.h>
//#include <stdlib.h>
// int main() {
//     char input[] = "Hello";
//     char *output = ft_strmapi(input, transform_char);

//     if (output) {
//         printf("Original string: %s\n", input);
//         printf("Transformed string: %s\n", output);
//         free(output);
//     } else {
//         printf("Memory allocation failed.\n");
//     }

//     return 0;
// }
