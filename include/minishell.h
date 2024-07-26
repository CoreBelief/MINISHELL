#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <signal.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <sys/types.h>

# define MAX_ARGS 1024

// Structs
typedef struct s_redirect
{
    int     type;  // 0: input, 1: output, 2: append, 3: heredoc
    char    *file;
} t_redirect;

typedef struct s_command
{
    char        *path;
    char        **argv;
    t_redirect  *redirects;
    int         redirect_count;
    int         pipe_in;
    int         pipe_out;
} t_command;

// Include builtins.h after struct definitions
# include "builtins.h"

// Global variables
extern int g_exit_status;
extern char **environ;

// Function prototypes

// Main loop
void    minishell_loop(void);

// Parsing and tokenization
t_command   *parse_command(char *input);
char        **tokenize_command(char *command);
char        *find_command_path(char *command);
int         is_redirection(char *token);
void        add_redirection(t_command *cmd, char *type, char *file);

// Command execution
void        execute_command(char *command);
void        update_exit_status(int status);

// Signal handling
void        setup_signals(void);
void        handle_sigint(int sig);
void        handle_sigquit(int sig);

// Environment variables
char        *expand_variables(char *str);

// Redirections and pipes
void        setup_redirections(t_command *cmd);
void        handle_pipes(t_command *cmd1, t_command *cmd2);

// Memory management
void        free_command(t_command *cmd);

// Error handling
void        print_error(char *msg);

#endif