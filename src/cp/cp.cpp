#include "cp.hpp"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

void cp(const char* src, const char* dst) 
{
    if(std::strcmp(src, dst) == 0) {
        std::cout << "cp '" << src << "' and '" << dst << "' are same file" << std::endl;
        return;
    }

    std::cout << "cp " << src << " " << dst << std::endl;
    int in_fd, out_fd, n_chars;
    char buffer[BUFFERSIZE];

    if((in_fd = open(src, O_RDONLY)) == -1) {
        oops("cannot open ", src);
    }

    if((out_fd = creat(dst, COPYMODE)) == -1) {
        oops("cannot create ", dst);
    }

    while(true) {

        n_chars = read(in_fd, buffer, sizeof(buffer));
        if(n_chars > 0) {
            if(write(out_fd, buffer, n_chars) != n_chars) {
                oops("write error to ", dst);
            }
        }else{
            if(n_chars == -1) {
                oops("read failed", src);
            }else{
                exit(1);
            }
        }       
    }

    if(close(in_fd) == -1 || close(out_fd) == -1) {
        oops("Error closing file", "");
    }
}   

void oops(const char* s1, const char* s2) {
    std::cout << "Error:" << s1 << " ";
    std::perror(s2);
    std::exit(1);
}