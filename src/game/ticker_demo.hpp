#ifndef TICKER_DEMO
#define TICKER_DEMO

#define SIGNAL_DEMO_INPUT_LEN 100

void countdown(int signal);
int setTicker(int n_msec);
void signalDemo();
void intHandler(int signal);
void quitHandler(int signal);
void sigactionDemo();

#endif