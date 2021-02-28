#ifndef TIME_SERV_2
#define TIME_SERV_2

#define PORTNUM 6790

#define HANDLE_ON 1
#define HANDLE_OFF 0

int SIG_HANDLE_STATUS = HANDLE_OFF;

void timeserv2();
void processRequest(int fd);
void processRequestWithFork(int fd);
void childWait(int signal);

#endif