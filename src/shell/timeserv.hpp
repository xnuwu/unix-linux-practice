#ifndef TIME_SERV
#define TIME_SERV

#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <netdb.h>
#include <arpa/inet.h>

#define PORTNUM 6790
#define HOSTLEN 256
#define oops(msg) {std::perror(msg);std::exit(1);}
void timeserv();

#endif