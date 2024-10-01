/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eeklund <eeklund@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 14:30:38 by eeklund           #+#    #+#             */
/*   Updated: 2023/10/17 12:15:22 by eeklund          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *str, int c, unsigned int n)
{
	size_t	i;
	char	*s;

	i = 0;
	s = (char *) str;
	while (i < n)
	{
		s[i] = c;
		i++;
	}
	return (s);
}

//#include <string.h>
// int main (void)
// {
// 	char ptr[10] = "stringeee";
// 	ft_memset(ptr, 48+256*2, 4);
// 	// printf("%lu\n",sizeof("05"));
// 	// printf("single: %lu\n",sizeof('5'));
// 	puts(ptr);
// }

// int main(void)
// {
// // 	// signal(SIGSEGV, sigsegv);  
// // 	// title("ft_memset\t: ")

// 	char tab[100];
// 	memset(tab, 0, 100);
// 	ft_memset(tab, 'A', 0);
// // 	/* 1 */ check(tab[0] == 0);
// 	ft_memset(tab, 'A', 42);
// 	printf("%c\n%c\n%c\n", tab[42], tab[41], tab[40]); 
// 	int i = 0;
// 	// for (; i < 100 && tab[i] == 'A'; ++i)
// 	while (i < 100 && tab[i] == 'A')
// 	{
// 		i++;
// 		printf("inside: %i\n", i); 
// 	}
//  	printf("%i\n", tab[42]); // == 0);
// 	printf("%i\n", i); // == 42 )
// // 	write(1, "\n", 1);
// // 	return (0);
// }