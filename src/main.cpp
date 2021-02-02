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

    std::cout << "open " << argv[1] << std::endl;
    fd = open(argv[1], O_WRONLY);
    if(fd == -1) {
        std::perror(argv[1]);
        std::exit(1);
    }

    long int w_chars;
    while(std::cin.getline(buf, BUFF_SIZE)) {
        w_chars = write(fd, buf, std::strlen(buf));
        write(fd, "\n", 1);
        if(w_chars == -1) {
            std::perror(argv[1]);
            break;
        }
    }

    close(fd);
}