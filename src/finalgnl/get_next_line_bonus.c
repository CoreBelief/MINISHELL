/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line_bonus.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/27 13:51:10 by eeklund       #+#    #+#                 */
/*   Updated: 2023/11/28 17:38:47 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*create_leftover(int fd, char *leftover)
{
	ssize_t	bytes_read;
	char	*buffer;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (free_secure(&leftover));
	bytes_read = 1;
	while ((!ft_strchr(leftover, '\n') && bytes_read > 0))
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free_secure(&leftover);
			return (free_secure(&buffer));
		}
		buffer[bytes_read] = '\0';
		leftover = ft_strjoin(leftover, buffer);
		if (!leftover)
			return (free_secure(&buffer));
	}
	free (buffer);
	return (leftover);
}

char	*create_line(char *leftover)
{
	int		i;
	char	*line;

	if (leftover[0] == '\0')
		return (NULL);
	i = 0;
	while (leftover[i] && leftover[i] != '\n')
		i++;
	if (leftover[i] == '\n')
		i++;
	line = malloc(i + 1);
	if (!line)
		return (NULL);
	i = 0;
	while (leftover[i] && leftover[i] != '\n')
	{
		line[i] = leftover[i];
		i++;
	}
	if (leftover[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

char	*update_leftover(char *leftover_old)
{
	int		i;
	int		len;
	char	*leftover_new;

	i = 0;
	while (leftover_old[i] && leftover_old[i] != '\n')
		i++;
	if (leftover_old[i] == '\0')
		return (free_secure(&leftover_old));
	len = ft_strlen(leftover_old) - i;
	leftover_new = malloc(len + 1);
	if (!leftover_new)
		return (free_secure(&leftover_old));
	ft_strlcpy(leftover_new, &leftover_old[++i], len);
	free_secure(&leftover_old);
	return (leftover_new);
}

char	*get_next_line(int fd)
{
	static char	*leftover[1024];
	char		*line;

	line = NULL;
	if (fd < 0 || BUFFER_SIZE < 0)
		return (NULL);
	leftover[fd] = create_leftover(fd, leftover[fd]);
	if (!leftover[fd])
		return (NULL);
	line = create_line(leftover[fd]);
	if (!line)
		return (free_secure(&leftover[fd]));
	leftover[fd] = update_leftover(leftover[fd]);
	return (line);
}

// int	main(void)
// {
// 	textfiles("test.txt");
// 	textfiles("1char.txt");
//  textfiles("1line.txt");
// 	textfiles("empty.txt");
// 	textfiles("1line_nl.txt");
// 	
// return (0);
// }

// int	main(void)
// {
// 	char	*line;
// 	int		i;
// 	int		fd;
// 	int		fd2;
// 	int		fd3;

// 	fd = open("test.txt", O_RDONLY);
// 	if (fd == -1)
// 	 	return (1);
// 	fd2 = open("test2.txt", O_RDONLY);
// 	if (fd3 == -1)
// 	 	return (1);
// 	fd3 = open("test3.txt", O_RDONLY);	
// 	if (fd3 == -1)
// 	 	return (1);
// 	line = get_next_line(fd);
// 	printf(">%s", line);
// 	free (line);

// 	line = get_next_line(fd2);
// 	printf(">%s", line);
// 	free (line);

// 	line = get_next_line(fd3);
// 	printf(">%s", line);
// 	free (line);

// 	line = get_next_line(fd);
// 	printf(">%s", line);
// 	free (line);

// 	line = get_next_line(fd2);
// 	printf(">%s", line);
// 	free (line);

// 	line = get_next_line(fd3);
// 	printf(">%s", line);
// 	free (line);

// }