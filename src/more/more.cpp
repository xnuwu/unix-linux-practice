//
// Created by xunwu on 2020/12/29.
//
#include "more.hpp"
#include <string>
#include <cstring>
#include <istream>
#include <termios.h>
#include <unistd.h>

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
    //终端无回现并直接读取输入无需回车
    static struct termios oldT, newT;
    tcgetattr(STDIN_FILENO, &oldT);
    newT = oldT;
    newT.c_lflag &= ~(ICANON);
    newT.c_lflag &= ~(ECHO);
    newT.c_cc[VMIN] = 1;
    newT.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &newT);

    char reply;
    std::cout << "\033[s";
    std::cout << "\n\033[7m more? \033[0m";

    while(std::cin.get(reply)) {

        std::cout << "\033[7D";
        std::cout << "\033[K";
        std::cout << "\033[u";
        
        tcsetattr(STDIN_FILENO, TCSANOW, &oldT);

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