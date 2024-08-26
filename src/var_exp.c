/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   var_exp.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/23 13:35:00 by eeklund       #+#    #+#                 */
/*   Updated: 2024/08/25 15:30:51 by elleneklund   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "var_exp.h"

int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

int	find_var_len(char *var)
{
	int	i;

	i = 0;
	while (var[i] && is_var_char(var[i]))
		i++;
	return (i);
}

char	*variable_exp(char *str, int *i)
{
	int		len;
	char	*expansion;
	char	*var;

	len = find_var_len(&str[*i]);
	var = ft_strndup(&str[*i], len);
	//error handling
	expansion = getenv(var);
	free (var);
	*i += len;
	return (expansion);

}

char	*append_str(char *og, char *to_append)
{
	char	*str;

	str = ft_strjoin(og, to_append);
	if (!str)
		return (NULL);
	return (str);
}

void	variable_exp_dollar(t_token *token, char *str)
{
	int		i;
	char	*expansion;
	char	*new_str;

	i = 0;
	expansion = NULL;
	new_str = ft_strdup("");
	//error handling
	if (str[i] == '$' && str[i + 1])
	{
		i++;
		expansion = variable_exp(str, &i);
		if (!expansion)
		{
			printf("expansion variable not found\n");
			free (token->content);
			token->content = "";
			return ;
		}
		new_str = append_str(new_str, expansion);
		new_str = append_str(new_str, &str[i]);
		token->content = new_str;
		free (str);
	}
}

int	until_dollar(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '$')
		i++;
	return (i);
}

void	variable_exp_double(t_token *token, char *str)
{
	int		i;
	int		len;
	char	*expansion;
	char	*new_str;

	i = 0;
	expansion = NULL;
	new_str = ft_strdup("");
	//error handling
	while (str[i])
	{
		if (str[i] == '$' && ft_isalnum(str[i + 1]))
		{
			i++;
			expansion = variable_exp(str, &i);
			if (!expansion)
			{
				printf("expansion variable not found\n");
				free (token->content);
				token->content = "";
				return ;
			}
			new_str = append_str(new_str, expansion);
			// i += len;
		}
		len = until_dollar(&str[i]);
		new_str = append_str(new_str, ft_strndup(&str[i], len));
		i += len;
		token->content = new_str;
	}
}


//OLD VERSION

// int	is_var_char(char c)
// {
// 	return (ft_isalnum(c) || c == '_');
// }

// int	find_var_len(char *var)
// {
// 	int	i;

// 	i = 0;
// 	while (var[i] && is_var_char(var[i]))
// 		i++;
// 	return (i);
// }

// void	variable_exp_dollar(t_token *token, char *str)
// {
// 	int		i;
// 	int		len;
// 	char	*expansion;
// 	char	*new_str;
// 	char	*var;

// 	i = 0;
// 	expansion = NULL;
// 	new_str = ft_strdup("");
// 	//error handling
// 	if (str[i] == '$' && str[i + 1])
// 	{
// 		i++;
// 		len = find_var_len(&str[i]);
// 		var = ft_strndup(&str[i], len);
// 		//error handling
// 		expansion = getenv(var);
// 		free (var);
// 		if (!expansion)
// 		{
// 			printf("expansion variable not found\n");
// 			free (token->content);
// 			token->content = "";
// 			return ;
// 		}
// 		new_str = ft_strjoin(new_str, expansion);
// 		if (!new_str)
// 			return ;
// 		i += len;
// 		len = ft_strlen(&str[i]);
// 		new_str = ft_strjoin(new_str, &str[i]);
// 		if (!new_str)
// 			return ;
// 		token->content = new_str;
// 		free (str);
// 	}
// }

// int	until_dollar(char *str)
// {
// 	int	i;

// 	i = 0;
// 	while (str[i] && str[i] != '$')
// 		i++;
// 	return (i);
// }

// void	variable_exp_double(t_token *token, char *str)
// {
// 	int		i;
// 	int		len;
// 	char	*expansion;
// 	char	*new_str;
// 	char	*var;

// 	i = 0;
// 	expansion = NULL;
// 	new_str = ft_strdup("");
// 	//error handling
// 	while (str[i])
// 	{
// 		if (str[i] == '$' && ft_isalnum(str[i + 1]))
// 		{
// 			i++;
// 			len = find_var_len(&str[i]);
// 			var = ft_strndup(&str[i], len);
// 			//error handling
// 			expansion = getenv(var);
// 			free (var);
// 			if (!expansion)
// 			{
// 				printf("expansion variable not found\n");
// 				free (token->content);
// 				token->content = "";
// 				return ;
// 			}
// 			new_str = ft_strjoin(new_str, expansion);
// 			if (!new_str)
// 				return ;
// 			i += len;
// 			len = until_dollar(&str[i]);
// 			char *middle = ft_strndup(&str[i], len);
// 			new_str = ft_strjoin(new_str, middle);
// 			if (!new_str)
// 				return ;
// 			free (middle);
// 			i += len;
// 			token->content = new_str;
// 		}
// 		else
// 			i++;
// 	}
// }
