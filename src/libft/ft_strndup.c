/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strndup.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/30 18:45:27 by eeklund       #+#    #+#                 */
/*   Updated: 2024/07/30 18:47:47 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s, int size)
{
	int		i;
	int		slen;
	char	*dup;

	i = 0;
	slen = ft_strlen(s);
	dup = (char *)malloc(sizeof(char) * (slen + 1));
	if (!dup)
		return (NULL);
	while (s[i] && i < size)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}