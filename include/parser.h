/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/31 16:10:11 by eeklund       #+#    #+#                 */
/*   Updated: 2024/07/31 16:19:27 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

typedef struct s_command
{
	char				*name;         // Command name
	char				**args;       // Command arguments (including name as args[0])
	t_redirection		*redirects;  // List of redirections
	struct s_command	*next;    	// Next command in pipeline or sequence
}	t_command;

typedef struct s_redirection
{
	int						type;                  // REDIR_IN, REDIR_OUT, REDIR_APPEND, REDIR_HEREDOC
	char					*filename;            // File to redirect to/from (or delimiter for heredoc)
	char					*heredoc_content;    // Content for heredoc (if applicable)
	struct s_redirection	*next;
}	t_redirection;

typedef struct s_pipeline
{
	t_command			*commands;       // Linked list of commands in this pipeline
	struct s_pipeline	*next;   		// Next pipeline in a sequence of pipelines
}	t_pipeline;

#endif
