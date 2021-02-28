#include "timeserv2.hpp"
#include "socketlib.cpp"
#include <time.h>
#include <string.h>
#include <wait.h>
#include <signal.h>

/**
 * description: time server using makeSocket
 **/
void timeserv2() {
    
    signal(SIGCHLD, childWait);

    int fd;
    int sockFd = makeServerSocket(PORTNUM);
    if(sockFd == -1) {
        std::cerr << "make server socket failed" << std::endl;
        std::exit(1);
    }

    while(true) {
        fd = accept(sockFd, NULL, NULL);
        if(fd == -1 && SIG_HANDLE_STATUS == HANDLE_OFF) {
            std::cerr << "accept failed" << std::endl;
            break;
        }else if(fd == -1 && SIG_HANDLE_STATUS == HANDLE_ON) {
            std::cerr << "sig handing, wait a minutes..." << std::endl;
            continue;
        }

        processRequestWithFork(fd);
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

/**
 * description: fork an new process
 *              then get server date and return by sockdFd
 **/
void processRequestWithFork(int sockFd) {
    int pid = fork();
    if(pid == 0) {
        dup2(sockFd, 1);
        close(sockFd);
        execl("/usr/bin/date", "date", NULL);
        oops("execl");
    }
}

/**
 * description: 子进程wait处理
 **/
void childWait(int signal) {
    SIG_HANDLE_STATUS = HANDLE_ON;
    std::cout << "child wait" << std::endl;
    while(waitpid(-1, NULL, WNOHANG));
    SIG_HANDLE_STATUS = HANDLE_OFF;
}