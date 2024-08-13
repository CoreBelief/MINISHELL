/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/13 18:19:51 by eeklund       #+#    #+#                 */
/*   Updated: 2024/08/13 18:28:11 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"
# include "tokenizer.h"

t_command	*init_cmd(void);
int			is_redirect_token(int token_type);
int			handle_redirection_parsing(t_command *cmd, t_token **token);
t_command	*parse_command_from_tokens(t_token *tokens);
void		print_cmd_list(t_command *head);

#endif