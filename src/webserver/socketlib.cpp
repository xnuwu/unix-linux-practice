#include "socketlib.hpp"
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <netdb.h>
#include <string.h>
#include <iostream>

/**
 * description: build a server listen at specific port, 
 *              this function will invoke makeServerSocketQ with backlog is 1
 * param: socket port
 * return: server socket sockFd or error -1
 **/
int makeServerSocket(int port) {
    return makeServerSocketQ(port, 1);
}

/**
 * description: build a server listen at specific port and backlog
 * param: socket port
 * return: server socket sockFd or error -1
 **/
int makeServerSocketQ(int port, int backlog) {
    struct sockaddr_in saddr;
    char hostname[HOST_LEN];
    hostent* hp;
    int sockFd;

    sockFd = socket(PF_INET, SOCK_STREAM, 0);
    if(sockFd == -1) {
        oops("socket");
        return -1;
    }

    memset(&saddr, 0, sizeof(saddr));
    gethostname(hostname, HOST_LEN);
    hp = gethostbyname(hostname);

    memcpy(&saddr.sin_addr, hp -> h_addr, hp -> h_length);
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    if(bind(sockFd, (struct sockaddr *) &saddr, sizeof(saddr)) != 0) {
        oops("bind");
        return -1;
    }

    if(listen(sockFd, backlog) != 0) {
        oops("listen");
        return -1;
    }

    return sockFd;
}

/**
 * description: connect to specific host:port and return socket fd
 * return: socket file descriptor or error -1
 **/
int connectToServer(const char* host, int port) {
    
    // std::cout << "connecting to " << host << ":" << port << std::endl;
    if(host == NULL) {
        std::cerr << "host required" << std::endl;
        return -1;
    }

    struct sockaddr_in saddr;
    hostent* hp;
    int sockFd;

    sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockFd == -1) {
        oops("socket");
        return -1;
    }

    memset(&saddr, 0, sizeof(saddr));
    hp = gethostbyname(host);
    if(hp == NULL) {
        std::cerr << "host invalid" << std::endl;
        return -1;
    }
    memcpy(&saddr.sin_addr, hp -> h_addr, hp -> h_length);
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);

    if(connect(sockFd, (struct sockaddr *) &saddr, sizeof(saddr)) != 0) {
        oops("connect");
        return -1;
    }

    return sockFd;
}