#include "rls.hpp"
#include <string.h>
#include <cstdlib>
#include <cstdio>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

/**
 * description: get result of `ls path` in specific host
 **/
void rls(const char* host, const char* path) {

    if(host == NULL || path == NULL) {
        std::cerr << "host and path was required" << std::endl;
        return;
    }

    std::cout << host << " " << path << std::endl;

    struct sockaddr_in saddr;
    hostent* hp;
    int sockId;
    char buf[BUFF_SIZE];
    size_t readNum;

    //create socket
    sockId = socket(AF_INET, SOCK_STREAM, 0);
    if(sockId == -1) {
        oops("socket");
    }

    //connnect to server
    memset(&saddr, 0, sizeof(saddr));
    hp = gethostbyname(host);
    if(hp == NULL) {
        oops("hostent");
    }

    memcpy(&saddr.sin_addr, hp -> h_addr, hp -> h_length);
    saddr.sin_port = htons(PORTNUM);
    saddr.sin_family = AF_INET;

    if(connect(sockId, (struct sockaddr *) &saddr, sizeof(saddr)) != 0) {
        oops("connect");
    }

    //write data
    if(write(sockId, path, strlen(path)) == -1) {
        oops("write");
    }

    if(write(sockId, "\n", 1) == -1) {
        oops("write end");
    }

    //print result
    while((readNum = read(sockId, buf, BUFF_SIZE)) > 0) {
        if(write(1, buf, readNum) == -1) {
            oops("write stdout");
        }
    }

    close(sockId);
}

/**
 * description: get path from connection and return `ls path`'s result by connnection
 **/
void rlsd() {

    struct sockaddr_in saddr;
    struct hostent* hp;
    char host[HOST_LEN];
    char command[BUFF_SIZE];
    char buf[BUFF_SIZE];
    int sockId, sockFd;
    FILE* inputFp;
    FILE* outputFp;
    FILE* pipeFp;

    //create socket
    sockId = socket(PF_INET, SOCK_STREAM, 0);
    if(sockId == -1) {
        oops("socket");
    }

    //resuse port
    int enable = 1;
    if(setsockopt(sockId, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) == -1) {
        oops("resuse port");
    }

    //bind address
    memset(&saddr, 0, sizeof(saddr));
    gethostname(host, HOST_LEN);
    hp = gethostbyname(host);
    memcpy(&saddr.sin_addr, hp -> h_addr, hp -> h_length);
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(PORTNUM);
    if(bind(sockId, (struct sockaddr*) &saddr, sizeof(saddr)) != 0) {
        oops("bind");
    }

    //listen
    if(listen(sockId, 1) != 0) {
        oops("listen");
    }

    struct sockaddr raddr;
    socklen_t raddrLen;
    while(true) {
        sockFd = accept(sockId, &raddr, &raddrLen);
        
        if(sockFd == -1) {
            oops("accept");
        }

        if(raddr.sa_family == AF_INET) {
            char ipStr[INET_ADDRSTRLEN];
            struct sockaddr_in* addrP = (struct sockaddr_in *) &raddr;
            inet_ntop(AF_INET, &(addrP -> sin_addr), ipStr, sizeof(ipStr));
            std::cout << "connect from " << ipStr << ":" << ntohs(addrP -> sin_port) << std::endl;
        }

        if((outputFp = fdopen(sockFd, "w")) == NULL) {
            oops("fdopen sockFd w");
        }

        if((inputFp = fdopen(sockFd, "r")) == NULL) {
            oops("fdopen sockFd r");
        }

        if(fgets(buf, BUFF_SIZE - 5, inputFp) == NULL) {
            oops("fgets");
        }
        sonitize(buf);
        sprintf(command, "ls %s", buf);

        if((pipeFp = popen(command, "r")) == NULL) {
            oops("popen");
        }
        
        int ch;
        while((ch = getc(pipeFp)) != EOF) {
            if(putc(ch, outputFp) == -1) {
                oops("fputs");
            }
        }
        fflush(outputFp);
        
        fclose(inputFp);
        fclose(outputFp);
        pclose(pipeFp);
    }
}

void sonitize(char* path) {
    char* dst, *src;
    src = path;
    dst = path;

    while(*src != '\0') {
        if(*src == '/' || isalnum(*src)) {
            *dst = *src;
            dst++;
        }
        src++;
    }

    *dst = '\0';
}