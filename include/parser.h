/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/13 18:19:51 by eeklund       #+#    #+#                 */
/*   Updated: 2024/09/19 21:00:38 by rdl           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

t_cmd	*init_cmd(void);
t_cmd	*parse_command_from_tokens(t_token *tokens, t_shell *shell);
void	print_cmd_list(t_cmd *head);
void	remove_quotes(char *str);

void	set_cmd_paths(t_cmd *cur_cmd);
t_cmd	*handle_pipe_parsing(t_cmd *cmd, int *i);
int		handle_arg_parsing(t_cmd *cmd, t_token *tokens, int *i);
int		handle_arg_parsing_2nd(t_cmd *cmd, t_token **tokens, int *i, t_shell *shell);

/* parser_redirects */
int		is_redirect_token(int token_type);
int		handle_heredoc_parsing(t_cmd *cmd, t_token **token, t_shell *shell);
int		handle_redirection_parsing(t_cmd *cmd, t_token **token);



#endif