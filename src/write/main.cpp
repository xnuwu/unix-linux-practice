#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define BUFF_SIZE 512

int main(int argc, char* argv[]) {
    int fd;
    char buf[512];

    if(argc != 2) {
        std::cerr << "usage: write0 ttyname" << std::endl;
    }

    fd = open(argv[1], O_RDONLY);
    if(fd == -1) {
        std::perror(argv[1]);
        std::exit(1);
    }

    
    while(std::cin.get(buf, BUFF_SIZE)) {
        if(write(fd, buf, std::strlen(buf)) == -1) {
            break;
        }
    }

    close(fd);
}