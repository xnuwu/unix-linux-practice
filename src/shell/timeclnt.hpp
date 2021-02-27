#ifndef TIME_CLNT
#define TIME_CLNT

#define oops(msg) {std::perror(msg);std::exit(1);}
#define BUFF_SIZE 256

#include <netinet/in.h>
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <cstdio>
#include <netdb.h>
#include <arpa/inet.h>
#include <iostream>

int timeclnt(const char* host, const char* port);

#endif