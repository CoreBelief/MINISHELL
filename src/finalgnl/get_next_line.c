/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/24 15:47:49 by elleneklund   #+#    #+#                 */
/*   Updated: 2024/10/01 22:20:11 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*create_leftover(int fd, char *leftover)
{
	ssize_t	bytes_read;
	char	*buffer;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (free_secure(&leftover));
	bytes_read = 1;
	while ((!ft_strchr_gnl(leftover, '\n') && bytes_read > 0))
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free_secure(&leftover);
			return (free_secure(&buffer));
		}
		buffer[bytes_read] = '\0';
		leftover = ft_strjoin_gnl(leftover, buffer);
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
	len = ft_strlen_gnl(leftover_old) - i;
	leftover_new = malloc(len + 1);
	if (!leftover_new)
		return (free_secure(&leftover_old));
	ft_strlcpy_gnl(leftover_new, &leftover_old[++i], len);
	free_secure(&leftover_old);
	return (leftover_new);
}

char	*get_next_line(int fd)
{
	static char	*leftover = NULL;
	char		*line;

	line = NULL;
	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	leftover = create_leftover(fd, leftover);
	if (!leftover)
		return (NULL);
	line = create_line(leftover);
	if (!line)
		return (free_secure(&leftover));
	leftover = update_leftover(leftover);
	return (line);
}

// int	test_textfiles(const char *filename)
// {
// 	int		fd;
// 	char	*line;

// 	line = NULL;
// 	fd = open(filename, O_RDONLY);
// 	printf("test: %s", filename);
// 	while ((line = get_next_line(fd)) != NULL)
// 	{
// 		printf(">%s", line);
// 		free (line);
// 	}
// 	printf("\n----------------------------------------\n");
// 	close(fd);
// 	return (0);
// }
// int	main(void)
// {
// 	// test_textfiles("test.txt");
// 	// test_textfiles("test2.txt");
//  // test_textfiles("test3.txt");
//	// test_textfiles("test4.txt");
//	// test_textfiles("test5.txt");
// return (0);
// }