// include/signal_handlers.h
#ifndef SIGNAL_HANDLERS_H
#define SIGNAL_HANDLERS_H

void setup_signals_shell(void);
void setup_signals_child(void);

void handle_signals_during_execution(void);

int check_eof(char *line);

int get_exit_status(void);

void reset_exit_status(void);

void reset_signals(void);

#endif
