/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expan_special.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/01 22:03:32 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/01 22:04:00 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_strcpy_replace(char *dst, char *src, char *old, char *new)
{
	int	i;
	int	j;
	int	old_len;

	i = 0;
	j = 0;
	old_len = ft_strlen(old);
	while (src[i])
	{
		if (ft_strncmp(&src[i], old, old_len) == 0)
		{
			ft_strcpy(&dst[j], new);
			i += old_len;
			j += ft_strlen(new);
		}
		else
			dst[j++] = src[i++];
	}
	dst[j] = '\0';
}

char	*replace_substr(char *str, char *old, char *new)
{
	char	*result;
	int		i;
	int		count;
	int		new_len;
	int		old_len;

	i = 0;
	count = 0;
	new_len = ft_strlen(new);
	old_len = ft_strlen(old);
	while (str[i])
	{
		if (ft_strncmp(&str[i], old, old_len) == 0)
		{
			count++;
			i += old_len;
		}
		else
			i++;
	}
	result = malloc(sizeof(char) * (i + count * (new_len - old_len) + 1));
	if (!result)
		return (NULL);
	ft_strcpy_replace(result, str, old, new);
	return (result);
}

char	*expand_exit(char *arg, t_shell *shell)
{
	char	*expanded;

	expanded = replace_substr(arg, "_EXIT_STATUS_", \
	ft_itoa(shell->last_exit_status));
	if (!expanded)
		return (NULL);
	return (expanded);
}