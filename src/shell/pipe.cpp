#include "pipe.hpp"
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

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