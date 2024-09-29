#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "minishell.h"

#define MAX_ARGS 1024

t_token	*tokenizer(char *input, t_shell *shell);
t_token	*add_token(t_token **head, char *content, t_token_type type);
void	tokenize_redirection(char *input, int *i, t_token **head);
void	tokenize_single(char *input, int *i, t_token **head);
void	tokenize_double(char *input, int *i, t_token **head);
int		tokenize_word(char *input, int *i, t_token **head, t_shell *shell);
int		is_whitespace(char c);
bool	is_special_token(char c);

void	print_token_list(t_token *head);

#endif