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

/**
    \033[0m		// 关闭所有属性
    \033[1m		// 设置为高亮
    \033[4m		// 下划线
    \033[5m		// 闪烁
    \033[7m		// 反显
    \033[8m		// 消隐
    \033[nA		// 光标上移 n 行
    \033[nB		// 光标下移 n 行
    \033[nC		// 光标右移 n 行
    \033[nD		// 光标左移 n 行
    \033[y;xH	// 设置光标位置
    \033[2J		// 清屏
    \033[K		// 清除从光标到行尾的内容
    \033[s		// 保存光标位置
    \033[u		// 恢复光标位置
    \033[?25l	// 隐藏光标
    \033[?25h	// 显示光标

 **/
int displayMore() 
{
    static struct termios oldT, newT;
    tcgetattr(STDIN_FILENO, &oldT);
    newT = oldT;

    //设置非标准模式
    newT.c_lflag &= ~(ICANON);

    //不回显输入
    newT.c_lflag &= ~(ECHO);

    //设置最少的读入数据量,当有一个字节数据的时候即可返回
    newT.c_cc[VMIN] = 1;

    //等待读入超时时间,读到数据后立刻返回
    newT.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &newT);

    char reply;
    std::cout << "\033[s";
    std::cout << "\n\033[7m more? \033[0m";

    while(std::cin.get(reply)) {

        //清空当前行
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