// src/main.c
#include "minishell.h"

int main(void) {
    // Initialize shell environment and signal handlers
    initialize_shell();
    setup_signal_handlers();
    
    // Main loop to display prompt and read input
    shell_loop();
    
    return 0;
}
