/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   commandline.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/12/12 10:23:42 by eeklund       #+#    #+#                 */
/*   Updated: 2023/12/12 10:44:35 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int	main(int av, char **ac)
{
	int	i;

	if (av < 1)
	{
		return (1);
	}
	i = 1;
	while (av > 1)
	{
		printf("%s\n", ac[i]);
		av--;
		i++;
	}
	return (0);
}
