#include "playAgain.hpp"
#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

/**
 * 终端属性配置,0-还原,1-保存
 **/
void ttyMode(int flag) {
    static termios originalTermiosInfo;
    static int originalFlag;

    if(flag == 1) {
        tcgetattr(0, &originalTermiosInfo);
        originalFlag = fcntl(0, F_GETFL);
    }else{
        tcsetattr(0, TCSANOW, &originalTermiosInfo);
        fcntl(0, F_SETFL, originalFlag);
    }
}

/**
 * 设置终端非标准、不回显模式
 **/
void set_cr_noecho_mode() {
    struct termios termiosInfo;
    tcgetattr(0, &termiosInfo);

    termiosInfo.c_lflag &= ~ICANON;
    termiosInfo.c_lflag &= ~ECHO;
    termiosInfo.c_cc[VMIN] = 1;

    tcsetattr(0, TCSANOW, &termiosInfo);
}

/**
 * 设置终端读取非延迟模式
 **/
void set_no_delay_mode() {
    int termFlag = fcntl(0, F_GETFL);
    termFlag |= O_NDELAY;
    fcntl(0, F_SETFL, termFlag);
}

/**
 * 获取用户中断输入响应是否继续,0-停止,1-继续
 **/
int getResponse() {
    int input;
    int maxTryTimes = TRIES;
    std::cout << QUESTION << std::endl;
    while(true) {
        sleep(SLEEPTIME);
        input = std::tolower(getOKChar());
        if(input == 'y') {
            return 1;
        }

        if(input == 'n') {
            return 0;
        }

        if(maxTryTimes-- == 0) {
            return 2;
        }
        BEEP;
    }
}

/**
 * 获取终端输入,直到获取正确的值,并返回.非法字符字节丢弃
 **/
int getOKChar() {
    int ch;
    while((ch = getchar()) != EOF && std::strchr("YyNn", ch) == NULL);
    return ch;
}