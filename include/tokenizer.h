/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/30 18:58:10 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/14 17:36:31 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "minishell.h"

# define MAX_ARGS 1024

int		is_whitespace(char c);
bool	is_special_token(char c);
int		prev_token_hd(t_token *tok);

int		no_quotes_state(char *input, char **res, int *i, t_shell *shell);
int		handle_quotes_state(char *input, char **res, int *i, t_shell *shell);
int		double_quotes_state(char *input, char **res, int *i, t_shell *shell);
int		single_quotes_state(char *input, char **res, int *i, t_shell *shell);

t_token	*add_token(t_token **head, char *content, t_token_type type);
int		tokenizer(char *input, t_shell *shell);


#endif