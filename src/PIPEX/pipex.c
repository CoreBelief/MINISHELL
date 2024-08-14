/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: skoene <skoene@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/16 16:54:21 by skoene        #+#    #+#                 */
/*   Updated: 2024/05/28 15:09:32 by skoene        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_PipexProcessInfo	process_info;
	int					status;

	status = 0;
	if (argc != 5)
	{
		ft_putstr_fd("Error: Bad arguments\n", 2);
		return (EXIT_FAILURE);
	}
	launch_processes(argv, envp, &process_info);
	waitpid(process_info.first_child_pid, NULL, 0);
	waitpid(process_info.second_child_pid, &status, 0);
	exit(WEXITSTATUS(status));
	return (0);
}
