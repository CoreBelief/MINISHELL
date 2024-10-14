/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/13 18:19:51 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/10 18:46:26 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

t_cmd	*init_cmd(void);
int		parse_command_from_tokens(t_shell *shell);
void	print_cmd_list(t_cmd *head);
void	remove_quotes(char *str);

void	set_cmd_paths(t_cmd *cur_cmd);
t_cmd	*handle_pipe_parsing(t_cmd *cmd, int *i);
int		handle_arg_parsing(t_cmd *cmd, t_token *tokens, int *i);
int		handle_word_parsing(t_cmd *cmd, t_token **tokens, int *i);

/* parser_redirects */
int		is_redirect_token(int token_type);
int		handle_redirection_parsing(t_cmd *cmd, t_token **token, t_shell *shell);

/* heredoc_parsing */
int		handle_heredoc_parsing(t_cmd *cmd, t_token **token, t_shell *shell);

/* heredoc_parsing_utils */
char	*create_filename(int redir_count);
int		open_hdfile(char *target);
int		write_to_hdfd(char *delim, t_shell *shell, int hdfd);
int		validate_heredoc_syntax(t_cmd *cmd, t_token **token, t_shell *shell);
char	*create_and_open_temp_file(int redirect_count, int *hered_fd);

#endif