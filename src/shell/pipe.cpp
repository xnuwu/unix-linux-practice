#include "pipe.hpp"
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstdio>
#include <string>

void testPipe() {

    int fd;
    int pipeFd[2];
    char buff[BUFF_SIZE];

    if(pipe(pipeFd) == -1) {
        std::cerr << "pipe open failed" << std::endl;
    }

    fd = fork();
    switch(fd) {

        case -1:
            std::cerr << "fork failed" << std::endl;
            break;

        case 0:
            while(true) {
                if(write(pipeFd[1], CHILD_MSG, strlen(CHILD_MSG)) != strlen(CHILD_MSG)) {
                    OOPS("write", 3);
                }
                sleep(5);
            }
            break;
        
        default:
            while(true) {
                if(write(pipeFd[1], PARENT_MSG, strlen(PARENT_MSG)) != strlen(PARENT_MSG)) {
                    OOPS("write", 4);
                }
                sleep(1);
                size_t len = read(pipeFd[0], buff, BUFF_SIZE);
                if(len < 0) {
                    break;
                }
                write(1, buff, len);
            }
            break;
    }
}

/**
 * 说明:测试通过pipe与自定义的程序/home/course/unix/bin/echo进行交互
 **/
void testPipe2() {

    int toEchoFD[2], fromEchoFD[2];

    if(pipe(toEchoFD) == -1 || pipe(fromEchoFD) == -1) {
        std::perror("pipe echo fd failed");
        std::exit(1);
    }

    int pid = fork();
    if(pid == -1) {
        std::perror("fork failed");
        std::exit(1);
    }else if(pid == 0) {
        //子进程调用echo
        if(dup2(toEchoFD[0], 0) == -1) {
            std::perror("dup toEchoFD failed");
            std::exit(1);
        }
        close(toEchoFD[0]);
        close(toEchoFD[1]);

        if(dup2(fromEchoFD[1], 1) == -1) {
            std::perror("dup fromEchoFD failed");
            std::exit(1);
        }
        close(fromEchoFD[0]);
        close(fromEchoFD[1]);

        execlp("/home/course/unix/bin/echo", "/home/course/unix/bin/echo", NULL);
        std::perror("execlp /home/course/unix/bin/echo failed");
        std::exit(1);

    }else {
        //父进程
        std::string data;
        char buf[BUFF_SIZE];
        FILE* input = fdopen(fromEchoFD[0], "r");
        FILE* output = fdopen(toEchoFD[1], "w");

        while(std::cout << "$>" && fgets(buf, BUFF_SIZE, stdin) != NULL) {
            if(fputs(buf, output) == -1) {
                std::cerr << "write failed" << std::endl;
                std::exit(1);
            }

            fflush(output);
            if(fgets(buf, BUFF_SIZE, input) != NULL) {
                std::cout << "receive: " << buf << std::endl;
            }
        }

        fclose(input);
        fclose(output);
    }
}