/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eeklund <eeklund@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 19:38:25 by eeklund           #+#    #+#             */
/*   Updated: 2023/10/22 17:39:58 by eeklund          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	int	rem;
	int	digit;

	rem = n / 10;
	digit = n % 10;
	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		rem = -rem;
		digit = -digit;
	}
	if (rem > 0)
		ft_putnbr_fd(rem, fd);
	ft_putchar_fd((char)digit + 48, fd);
}

//int main(void)
//{
//	ft_putnbr_fd(5, 1);
//}