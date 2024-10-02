/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eeklund <eeklund@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 10:19:23 by elleneklund       #+#    #+#             */
/*   Updated: 2023/10/17 12:27:29 by eeklund          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*joinedstring;
	size_t	strlen1;
	size_t	i;
	size_t	j;

	strlen1 = ft_strlen(s1);
	joinedstring = (char *) malloc (strlen1 + ft_strlen(s2) + 1);
	if (!joinedstring)
		return (NULL);
	i = 0;
	j = 0;
	while (i < (strlen1 + ft_strlen(s2)))
	{
		if (i < (strlen1))
			joinedstring[i] = s1[i];
		else
		{
			joinedstring[i] = s2[j];
			j++;
		}
		i++;
	}
	joinedstring[i] = '\0';
	return (joinedstring);
}

// #include <stdio.h>
// int main(void)
// {
// 	char string[10] = "hellooo";
// 	char *joined = ft_strjoin(string, " goodbye");
// 	puts(joined);
// 	free (joined);
// }