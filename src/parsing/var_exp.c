/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   var_exp.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/23 13:35:00 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/04 18:12:06 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "var_exp.h"

int			handle_exit_status(char **new_str, int *i, t_shell *shell);
int			handle_variable(char **new_str, char *str, int *i, t_shell *shell);
int			handle_dollar(char **new_str, char *str, int *i, t_shell *shell);
int			handle_non_dollar(char **new_str, char *str, int *i);
static int	process_char(char **new_str, char *str, int *i, t_shell *shell);
char		*variable_exp_double(char *str, t_shell *shell);

int	handle_exit_status(char **new_str, int *i, t_shell *shell)
{
	char	*expansion;
	char	*tmp;

	(*i) += 2;
	expansion = ft_itoa(shell->last_exit_status);
	if (!expansion)
	{
		free(*new_str);
		return (0); //malloc
	}
	tmp = *new_str;
	*new_str = append_str(tmp, expansion);
	free(tmp);
	free(expansion);
	if (!*new_str)
		return (0); //malloc
	return (1);
}

int	handle_dollar(char **new_str, char *str, int *i, t_shell *shell)
{
	char	*tmp;
	char	*substr;

	if (str[*i + 1] == '?')
		return (handle_exit_status(new_str, i, shell)); // always malloc fail
	else if (ft_isalnum(str[*i + 1]) || str[*i + 1] == '_')
		return (handle_variable(new_str, str, i, shell)); // always malloc fail
	else //if (str[*i + 1] != '\'' || str[*i + 1] != '"')
	{
		tmp = *new_str;
		substr = ft_strndup(&str[*i], 1);
		if (!substr)
		{
			free (tmp);
			return (0); //malloc
		}
		*new_str = append_str(tmp, substr);
		free (tmp);
		free (substr);
		if (!*new_str)
			return (0); // malloc
	}
	(*i)++;
	return (1);
}

int	handle_non_dollar(char **new_str, char *str, int *i)
{
	int		len;
	char	*tmp;
	char	*substr;

	len = until_dollar(&str[*i]);
	if (len != 0)
	{
		tmp = *new_str;
		substr = ft_strndup(&str[*i], len);
		if (!substr)
		{
			free (tmp);
			return (0); // malloc
		}
		*new_str = append_str(tmp, substr);
		free (tmp);
		free (substr);
		if (!*new_str)
			return (0); // malloc
	}
	*i += len;
	return (1);
}

static int	process_char(char **new_str, char *str, int *i, t_shell *shell)
{
	char	*tmp;

	if (*i > 0 && str[*i - 1] == '\\' && str[*i] == '$')
	{
		if (!handle_non_dollar(new_str, str, i)) // malloc fail always
			return (0);
	}
	else if (str[*i] == '$')
	{
		tmp = *new_str;
		if (!handle_dollar(new_str, str, i, shell)) // always malloc fail
			return (0);
		if (tmp == *new_str)
			return (2);
	}
	else if (!handle_non_dollar(new_str, str, i)) // malloc fail always
		return (0);
	return (1);
}

char	*variable_exp_double(char *str, t_shell *shell)
{
	int		i;
	char	*new_str;
	int		result;

	i = 0;
	new_str = ft_strdup("");
	if (!new_str)
		return (NULL); // malloc fail
	while (str[i])
	{
		result = process_char(&new_str, str, &i, shell); 
		if (result == 0)
			return (NULL); // malloc fail always
		if (result == 2)
			return (new_str);
	}
	return (new_str);
}
