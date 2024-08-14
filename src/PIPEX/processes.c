/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   processes.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: skoene <skoene@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/25 14:57:38 by skoene        #+#    #+#                 */
/*   Updated: 2024/05/28 15:08:17 by skoene        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	first_child_process(char **argv, char **envp, int *fd)
{
	int	filein;

	close(fd[0]);
	filein = open(argv[1], O_RDONLY, 0777);
	if (filein == -1)
		error();
	dup2(fd[1], STDOUT_FILENO);
	dup2(filein, STDIN_FILENO);
	close(filein);
	close(fd[1]);
	execute(argv[2], envp);
}

void	second_child_process(char **argv, char **envp, int *fd)
{
	int	fileout;

	close(fd[1]);
	fileout = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fileout == -1)
		error();
	dup2(fd[0], STDIN_FILENO);
	dup2(fileout, STDOUT_FILENO);
	close(fileout);
	close(fd[0]);
	execute(argv[3], envp);
}

void	launch_processes(char **argv, char **envp,
		t_PipexProcessInfo *process_info)
{
	if (pipe(process_info->pipe_fds) == -1)
	{
		ft_putstr_fd("Error: Failed to create pipe.\n", 2);
		exit(EXIT_FAILURE);
	}
	process_info->first_child_pid = fork();
	if (process_info->first_child_pid == -1)
	{
		ft_putstr_fd("Error: Failed to fork first child process.\n", 2);
		exit(EXIT_FAILURE);
	}
	if (process_info->first_child_pid == 0)
		first_child_process(argv, envp, process_info->pipe_fds);
	process_info->second_child_pid = fork();
	if (process_info->second_child_pid == -1)
	{
		ft_putstr_fd("Error: Failed to fork second child process.\n", 2);
		exit(EXIT_FAILURE);
	}
	if (process_info->second_child_pid == 0)
		second_child_process(argv, envp, process_info->pipe_fds);
	close(process_info->pipe_fds[0]);
	close(process_info->pipe_fds[1]);
}
