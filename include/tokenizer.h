#ifndef TOKENIZER_H
# define TOKENIZER_H

char	**tokenize_command(char *command);

typedef enum e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
    TOKEN_REDIRECT_APPEND,
    TOKEN_HEREDOC,
    TOKEN_SINGLE_QUOTE,
    TOKEN_DOUBLE_QUOTE,
    TOKEN_DOLLAR,
    TOKEN_SPACE,
    TOKEN_NEWLINE
} token_type;

typedef struct s_token
{
	token_type	type;
	char		*content;
	struct s_token		*next;
}	t_token;

/* tokenizer */
t_token *tokenizer(char *input)

#endif