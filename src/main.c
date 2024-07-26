#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

// Global variable to store the exit status
int g_exit_status = 0;

void minishell_loop(void)
{
    char *line;

    printf("DEBUG: Entering minishell loop\n");
    
    // Set up signal handlers
    setup_signals();
    printf("DEBUG: Signals set up\n");

    while (1)
    {
        // Display prompt and read input
        line = readline("minishell> ");
        
        // Check if EOF (Ctrl+D) was encountered
        if (!line)
        {
            printf("DEBUG: EOF detected, exiting shell\n");
            printf("exit\n");
            break;
        }

        // Process non-empty input
        if (*line)
        {
            printf("DEBUG: Processing input: '%s'\n", line);
            
            // Add input to history
            add_history(line);
            printf("DEBUG: Added to history\n");
            
            // Execute the command
            execute_command(line);
            printf("DEBUG: Command execution completed\n");
        }
        else
        {
            printf("DEBUG: Empty input, continuing\n");
        }

        // Free the allocated memory for the input line
        free(line);
        printf("DEBUG: Freed input line\n");
    }

    printf("DEBUG: Exiting minishell loop\n");
}

int main(void)
{
    printf("DEBUG: Starting minishell\n");
    
    // Run the main shell loop
    minishell_loop();
    
    printf("DEBUG: Minishell loop exited\n");
    printf("DEBUG: Final exit status: %d\n", g_exit_status);
    
    return (g_exit_status);
}