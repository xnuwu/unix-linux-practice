//
// Created by xunwu on 2020/12/29.
//
#include "more.hpp"
#include <string>
#include <cstring>
#include <istream>

void displayContent(std::istream& is) 
{
    char line[LINELEN];
    int lineNumbers = 0;
    int reply = 0;

    while(lineNumbers <= PAGELEN) {

        if(lineNumbers == PAGELEN) {
            reply = displayMore();
            lineNumbers -= reply;

            if(lineNumbers == PAGELEN) {
                std::cout << "read exit -1" << std::endl;
                exit(-1);
            }
        }

        while(is.peek() == '\n') {
            is.get();
            std::cout << std::endl;
        }

        if(is.get(line, LINELEN, '\n')) {
            std::cout << line;
            lineNumbers++;
        }else{
            std::cout << "read finished with " << is.fail() << " " << is.bad() << " " << is.eof() << std::endl;
            std::cout << "error " << std::strerror(errno) << std::endl;
            exit(-2);
        }
    }
}

int displayMore() 
{
    char reply;
    std::cout << "\n\033[7m more? \033[m";
    while(std::cin.get(reply)) {
        if('q' == reply) {
            return 0;
        }
        
        if(' ' == reply) {
            return PAGELEN;
        }

        if('\n' == reply) {
            return 1;
        }
    } 
}