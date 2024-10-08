/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/30 18:58:10 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/08 15:15:16 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "minishell.h"

# define MAX_ARGS 1024

int		is_whitespace(char c);
bool	is_special_token(char c);
int		prev_token_hd(t_token *tok);

/* tokenize_word */
int		extract_and_append(char *input, int len, char **res, t_shell *shell);
int		no_quotes_state(char *input, char **res, int *i, t_shell *shell);
int		handle_quotes_state(char *input, char **res, int *i, t_shell *shell);
int		double_quotes_state(char *input, char **res, int *i, t_shell *shell);
int		single_quotes_state(char *input, char **res, int *i, t_shell *shell);

int		tokenize_word(char *input, int *i, t_token **head, t_shell *shell);
t_token	*add_token(t_token **head, char *content, t_token_type type);
int		tokenize_redirection(char *input, int *i, t_token **head);
int		tokenizer(char *input, t_shell *shell);

void	print_token_list(t_token *head);

#endif
// void	tokenize_single(char *input, int *i, t_token **head);
// void	tokenize_double(char *input, int *i, t_token **head);