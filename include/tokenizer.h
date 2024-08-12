#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "minishell.h"
#include "libft.h"

#define MAX_ARGS 1024

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

t_token *tokenizer(char *input);
t_token *add_token(t_token **head, char *content, t_token_type type);
void free_tokens(t_token **head);
int is_whitespace(char c);
void handle_redirection(char *input, int *i, t_token **head);
void handle_single(char *input, int *i, t_token **head);
void handle_double(char *input, int *i, t_token **head);
void handle_word(char *input, int *i, t_token **head);
bool is_special_token(char c);
void print_token_list(t_token *head);

#endif