/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eeklund <eeklund@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 14:41:01 by eeklund           #+#    #+#             */
/*   Updated: 2023/10/19 09:40:03 by eeklund          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	int		i;
	int		slen;
	char	*dup;

	i = 0;
	slen = ft_strlen(s);
	dup = (char *)malloc(sizeof(char) * (slen + 1));
	if (!dup)
		return (NULL);
	while (s[i])
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

// #include <string.h>
// int main(void)
// {
// // 	char *duplicate = ft_strdup("heeeej");
// // 	printf("%s", duplicate);
// // 	free (duplicate);
// char * s = ft_strdup((char*)"coucou");
// /* 2 */printf("s: %s\n len: %lu\n", s, strlen("coucou") + 1); free(s);

// }