/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: skoene <skoene@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/26 14:12:24 by skoene        #+#    #+#                 */
/*   Updated: 2024/05/28 14:12:27 by skoene        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "./libft/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <sys/wait.h>

typedef struct s_pipex_process_info
{
	int		pipe_fds[2];
	pid_t	first_child_pid;
	pid_t	second_child_pid;
}			t_PipexProcessInfo;

void		error(void);
char		**get_paths(char **envp);
char		*construct_path(char *directory, char *cmd);
char		*find_path(char *cmd, char **envp);
void		execute(char *argv, char **envp);

void		ft_split_free(char **arr);

void		first_child_process(char **argv, char **envp, int *fd);
void		second_child_process(char **argv, char **envp, int *fd);
void		launch_processes(char **argv, char **envp,
				t_PipexProcessInfo *process_info);

#endif
