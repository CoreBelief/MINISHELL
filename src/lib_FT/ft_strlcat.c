/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eeklund <eeklund@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 14:01:33 by eeklund           #+#    #+#             */
/*   Updated: 2023/10/12 14:41:51 by eeklund          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	destlen;
	size_t	srclen;

	destlen = ft_strlen(dst);
	srclen = ft_strlen(src);
	if (!src)
		return (destlen);
	if (size == 0)
		return (srclen);
	if (destlen >= size)
		return (size + srclen);
	i = 0;
	while (src[i] && (i + destlen + 1 < size))
	{
		dst[destlen + i] = src[i];
		i++;
	}
	dst[destlen + i] = '\0';
	return (destlen + srclen);
}
