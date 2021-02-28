#ifndef TIME_SERV_2
#define TIME_SERV_2

#define PORTNUM 6790

void timeserv2();
void processRequest(int fd);
void processRequestWithFork(int fd);

#endif