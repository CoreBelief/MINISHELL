#ifndef TOKENIZER_H
# define TOKENIZER_H

char	**tokenize_command(char *command);

typedef enum e_token_type
{
	TOKEN_WORD, //command and arguments 
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN, // <
	TOKEN_REDIRECT_OUT, // >
	TOKEN_REDIRECT_APPEND, // >>
	TOKEN_HEREDOC, // <<
	TOKEN_SINGLE_QUOTE,
	TOKEN_DOUBLE_QUOTE,
	// TOKEN_DOLLAR,
	TOKEN_NEWLINE
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*content;
	struct s_token	*next;
}	t_token;

/* tokenizer */
t_token	*tokenizer(char *input);
t_token	*add_token(t_token **head, char *content, t_token_type type);
int		is_whitespace(char c);

/* tokenizer_utils */
void	handle_redirection(char *input, int *i, t_token **head);
void	handle_single(char *input, int *i, t_token **head);
void	handle_double(char *input, int *i, t_token **head);
// void	handle_dollar();
void	handle_word(char *input, int *i, t_token **head);
void	skip_whitespace(char *input, int *i);
void	print_token_list(t_token *head);

#endif