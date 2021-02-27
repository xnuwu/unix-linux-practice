#include "timeserv.hpp"
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <iostream>

/**
 * 说明: socket时间服务
 **/
void timeserv() {

    struct sockaddr_in saddr;
    struct hostent* hp;
    char hostname[HOSTLEN];
    int sockId, sockFd;
    FILE* sockFp;
    time_t thetime;

    //创建socket
    sockId = socket(PF_INET, SOCK_STREAM, 0);
    if(sockId == -1) {
        oops("socket");
    }

    //绑定端口地址
    memset(&saddr, 0, sizeof(saddr));
    gethostname(hostname, HOSTLEN);
    hp = gethostbyname(hostname);

    bcopy((void*) hp -> h_addr, (void*)&saddr.sin_addr, hp->h_length);
    saddr.sin_port = htons(PORTNUM);
    saddr.sin_family = AF_INET;

    if(bind(sockId, (struct sockaddr *) &saddr, sizeof(saddr)) != 0) {
        oops("bind");
    }

    //监听端口
    if(listen(sockId, 1) != 0) {
        oops("listen");
    }
    std::cout << "timerser listen at " << PORTNUM << std::endl;

    //接受请求
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

        sockFp = fdopen(sockFd, "w");

        thetime = time(NULL);
        fprintf(sockFp, "The time here is..");
        fprintf(sockFp, "%s", ctime(&thetime));
        fclose(sockFp);
    }
}