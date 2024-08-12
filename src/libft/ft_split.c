/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eeklund <eeklund@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 17:50:57 by eeklund           #+#    #+#             */
/*   Updated: 2023/10/19 09:34:24 by eeklund          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	wordcounter(char const *s, char c)
{
	int	i;
	int	wcount;

	i = 0;
	wcount = 0;
	while (s[i])
	{
		if (s[i] == c)
			i++;
		else
		{
			wcount++;
			while (s[i] != c && s[i])
				i++;
		}
	}
	return (wcount);
}

static size_t	word_len(const char *start, char c)
{
	size_t	i;

	i = 0;
	while (start[i] != c && start[i])
		i++;
	return (i);
}

static char	**free_all(char **stringlist, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		free (stringlist[i]);
		i++;
	}
	free(stringlist);
	stringlist = NULL;
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**strings;
	int		i;
	int		words;

	if (!s)
		return (NULL);
	strings = (char **)malloc(sizeof(char *) * (wordcounter(s, c) + 1));
	if (!strings)
		return (NULL);
	i = 0;
	words = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i])
		{
			strings[words++] = ft_substr(s, i, word_len(&s[i], c));
			if (!strings[words - 1])
				return (free_all(strings, words - 1));
			i += word_len(&s[i], c);
		}
	}
	strings[words] = NULL;
	return (strings);
}

// #include <stdio.h>

// int main(void)
// {
// 	int i;
// 	char **stringarray;

// 	i = 0;
// 	stringarray = ft_split("-hej ---- jor --", '-');
// 	while (stringarray[i])
// 	{
// 		printf("string:%s\n", stringarray[i]);
// 		i++;
// 	}
// 	i = 0;
// 	// while (i < 3)
// 	// {
// 	// 	free(stringarray[i]);
// 	// 	// stringarray[i] = 0;
// 	// 	i++;
// 	// }
// 	// free (stringarray);
// 	free_all(stringarray, 3);
// 	return (0);
// }
