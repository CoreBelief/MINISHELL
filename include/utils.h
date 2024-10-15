/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/10 17:19:32 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/10 17:19:36 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H
#include "minishell.h"

char	*ft_strjoin3(const char *s1, const char *s2, const char *s3);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strndup(const char *s1, size_t n);
char	*ft_strcpy(char *dest, const char *src);
char	*ft_strncpy(char *dest, const char *src, size_t n);

#endif