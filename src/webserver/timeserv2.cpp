#include "timeserv2.hpp"
#include "socketlib.cpp"
#include "time.h"
#include <string.h>

/**
 * description: time server using makeSocket
 **/
void timeserv2() {
    int fd;
    int sockFd = makeServerSocket(PORTNUM);
    if(sockFd == -1) {
        std::cerr << "make server socket failed" << std::endl;
        std::exit(1);
    }

    while(true) {
        fd = accept(sockFd, NULL, NULL);
        if(fd == -1) {
            std::cerr << "accept failed" << std::endl;
            break;
        }
        processRequest(fd);
        close(fd);
    }
}

/**
 * description: get server date and return by sockdFd
 **/
void processRequest(int sockFd) {
    time_t now = time(NULL);
    char* timeStrP = ctime(&now);
    write(sockFd, timeStrP, strlen(timeStrP));
}