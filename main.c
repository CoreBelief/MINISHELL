/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/25 15:31:46 by eeklund       #+#    #+#                 */
/*   Updated: 2024/07/25 15:45:35 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

void	minishell_loop(void)
{
	char	*line; //malloced 

	line = NULL;
	while (1)
	{
		line = readline("something: ");
	}
}

int	main(int ac, char **av)
{
	minishell_loop();
}

