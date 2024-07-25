// src/prompt.c
#include "prompt.h"

void shell_loop() {
    while (1) {
        char *input = display_prompt();
        // Process input
        free(input);
    }
}

char* display_prompt() {
    // Display the prompt and read user input
    return readline("minishell$ ");
}
