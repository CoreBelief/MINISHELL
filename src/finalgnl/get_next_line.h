/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/24 15:52:23 by elleneklund   #+#    #+#                 */
/*   Updated: 2024/10/01 22:19:30 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H

# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10

# endif

# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>

char	*create_leftover(int fd, char *leftover);
char	*create_line(char *leftover);
char	*update_leftover(char *leftover_old);
char	*get_next_line(int fd);
char	*ft_strchr_gnl(const char *s, int c);
char	*ft_strjoin_gnl(char *s1, char const *s2);
size_t	ft_strlen_gnl(const char *str);
size_t	ft_strlcpy_gnl(char *dst, const char *src, size_t size);
int		test_textfiles(const char *filename);
char	*free_secure(char **str);

#endif