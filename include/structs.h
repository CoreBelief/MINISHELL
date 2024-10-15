/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   structs.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/08 15:16:58 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/15 19:05:32 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include "minishell.h"

typedef struct s_builtin
{
	const char	*name;
	int			is_parent;
}	t_builtin;

typedef enum token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_HEREDOC,
	TOKEN_SINGLE_QUOTE,
	TOKEN_DOUBLE_QUOTE,
	TOKEN_EXIT
}	t_token_type;

typedef struct s_token
{
	char			*content;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_redirect
{
	int		type;
	char	*file;
}	t_redirect;

typedef struct s_cmd
{
	char				**argv;
	t_redirect			*redir;
	int					redirect_count;
	int					input;
	int					output;
	int					pipe_in;
	int					pipe_out;
	int					expansion;
	struct s_cmd		*next;
}	t_cmd;

typedef struct s_shell
{
	t_token		*tokens;
	t_cmd		*commands;
	int			last_exit_status;
	char		**env;
	int			env_size;
}	t_shell;

#endif