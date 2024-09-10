// // include/signal_handlers.h
// #ifndef SIGNAL_HANDLERS_H
// #define SIGNAL_HANDLERS_H

// void setup_signals_shell(void);
// void setup_signals_child(void);

// void handle_signals_during_execution(void);

// int check_eof(char *line);

// int get_exit_status(void);

// void reset_exit_status(void);

// void reset_signals(void);

// #endif

#ifndef SIGNALS_H
# define SIGNALS_H

# include <signal.h>
# include <stdbool.h>

# define SIGINT_EXIT_STATUS 130
# define SIGQUIT_EXIT_STATUS 131

int get_and_reset_signal(void);

void handle_pending_signals(void);
// Global variable to store received signal
extern volatile sig_atomic_t g_received_signal;

// Function to set up signal handlers for the shell
void setup_signals_shell(void);

// Function to set up default signal handlers for child processes
void setup_signals_child(void);

// Function to get the received signal
int get_signal_received(void);

// Function to set whether a foreground process is running
void set_foreground_process(bool is_foreground);

// Function to handle signals during command execution
void handle_signals_during_execution(void);

// Function to update the exit status based on the signal
void update_exit_status(int status);

#endif /* SIGNALS_H */