/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line_utils.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/29 10:46:46 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/01 22:20:44 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen_gnl(const char *str)
{
	size_t	len;

	len = 0;
	if (!str)
		return (len);
	while (str[len])
		len++;
	return (len);
}

size_t	ft_strlcpy_gnl(char *dst, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (0);
}

char	*ft_strchr_gnl(const char *s, int c)
{
	int	i;

	i = 0;
	if (!s)
		return (NULL);
	while (s[i])
	{
		if (s[i] == (char)c)
			return ((char *)&s[i]);
		i++;
	}
	return (NULL);
}

char	*ft_strjoin_gnl(char *s1, char const *s2)
{
	char	*joined;
	size_t	i;
	size_t	j;
	size_t	len1;
	size_t	len2;

	len1 = ft_strlen_gnl(s1);
	len2 = ft_strlen_gnl(s2);
	joined = (char *) malloc (len1 + len2 + 1);
	if (!joined)
		return (free_secure(&s1));
	i = 0;
	j = 0;
	while (i < (len1))
	{
		joined[i] = s1[i];
		i++;
	}
	while (i < (len1 + len2))
		joined[i++] = s2[j++];
	joined[i] = '\0';
	free_secure(&s1);
	return (joined);
}

char	*free_secure(char **str)
{
	free(*str);
	*str = NULL;
	return (NULL);
}
