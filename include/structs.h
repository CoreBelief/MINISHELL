#ifndef STRUCTS_H
# define STRUCTS_H

# include "minishell.h"

//TOKENIZER

typedef enum {
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_HEREDOC,
	TOKEN_SINGLE_QUOTE,
	TOKEN_DOUBLE_QUOTE
} t_token_type;

typedef struct s_token
{
	char			*content;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

//PARSER
typedef struct s_redirect
{
	int		type;  // 0: input, 1: output, 2: append, 3: heredoc
	char	*file;
}	t_redirect;

typedef struct s_command
{
	char				*path;
	char				**argv;
	t_redirect			*redir;
	int					redirect_count;
	int					pipe_in;
	int					pipe_out;
	struct s_command	*next;
}	t_command;

typedef struct s_parsed_line
{
	t_command	*command;
	char		**env_vars;
}	t_parsed_line;

#endif