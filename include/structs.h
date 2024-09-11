#ifndef STRUCTS_H
# define STRUCTS_H

# include "minishell.h"

//TOKENIZER
// char *find_executable(char *command);

typedef struct s_builtin_command {
    const char *name;
    int is_parent;
} t_builtin_command;

// Define an array of built-in commands with a flag indicating if it is a "parent" command.
static const t_builtin_command builtin_commands[] = {
    {"cd", 1},
    {"pwd", 0},
    {"export", 1},
    {"echo", 0},
    {"unset", 1},
    {"env", 0},
    {"exit", 1},
    {"bye", 1},
    {NULL, 0}
};

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
	int					input;
	int					output;
	int					pipe_in;
	int					pipe_out;
	struct s_command	*next;
}	t_command;

typedef struct s_parsed_line
{
	t_command	*command;
	char		**env_vars;
}	t_parsed_line;

typedef struct s_shell
{
	t_command	*commands;
	int			last_exit_status;
	char		**env;
	int			env_size;
}	t_shell;

#endif