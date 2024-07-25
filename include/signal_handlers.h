// include/signal_handlers.h
#ifndef SIGNAL_HANDLERS_H
#define SIGNAL_HANDLERS_H

void setup_signal_handlers(void);
void handle_sigint(int sig);
void handle_sigquit(int sig);

#endif
