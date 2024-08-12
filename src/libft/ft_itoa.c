/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eeklund <eeklund@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 11:27:06 by elleneklund       #+#    #+#             */
/*   Updated: 2023/10/19 09:00:27 by eeklund          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	countnum(long n)
{
	int	count;

	if (n == 0)
		return (1);
	count = 0;
	if (n < 0)
	{
		count++;
		n = -n;
	}
	while (n > 0)
	{
		n = n / 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char	*intstring;
	int		len;
	long	num;
	int		i;

	num = (long)n;
	len = countnum(num);
	intstring = (char *) malloc ((len + 1) * (sizeof(char)));
	if (!intstring)
		return (NULL);
	i = 0;
	if (num < 0)
		num = -num;
	intstring[len] = '\0';
	while (i < len)
	{
		intstring[len - 1 - i] = (num % 10) + '0';
		num = num / 10;
		i++;
	}
	if (n < 0)
		intstring[0] = '-';
	return (intstring);
}

// #include <stdio.h>

// int main(void)
// {
// 	printf("result:%s", ft_itoa(500));

// }