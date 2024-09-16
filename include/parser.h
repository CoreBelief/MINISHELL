/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/13 18:19:51 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/14 13:59:43 by elleneklund   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

t_command	*init_cmd(void);
int			is_redirect_token(int token_type);
int			handle_redirection_parsing(t_command *cmd, t_token **token);
t_command	*parse_command_from_tokens(t_token *tokens, t_shell *shell);
void		print_cmd_list(t_command *head);

void		set_command_paths(t_command *cur_cmd);
t_command	*handle_pipe_parsing(t_command *cmd, int *i);
int			handle_arg_parsing(t_command *cmd, t_token *tokens, int *i);
int			handle_arg_parsing_2nd(t_command *cmd, t_token **tokens, int *i, t_shell *shell);
int			handle_heredoc_parsing(t_command *cmd, t_token **token, t_shell *shell);




#endif