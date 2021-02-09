#include "bounceld.hpp"
#include "signal.h"
#include "sys/time.h"
#include "curses.h"
#include <iostream>
#include <climits>

int col;    //列号
int row;    //行号
int dir;    //方向1向右，-1向左
int ndelay; //时钟更新速度
int delay;  //时钟更新速度

std::string message;
size_t messageLen;  //字符串长度
std::string blank;  //字符串空白填充，用于清除输出


/**
 * 用户控制消息加速、减速、方向进行移动
 **/
void bounce(const std::string& msg) {

    message = msg;
    messageLen = message.length();
    blank = std::string(messageLen, ' ');

    col = 0;
    row = 10;
    dir = 1;
    delay = 400;
    int ch; //用户输入

    initscr();
    crmode();   //禁用行缓存直接获取输入字符
    clear();
    noecho();

    move(row, col);
    addstr(message.c_str());
    signal(SIGALRM, bounceMoveMessage);
    bounceSetTicker(delay);

    while(true) {
        
        ndelay = 0;
        ch = getch();
        
        //退出
        if(ch == 'Q' || ch == 'q') {
            break;
        }

        //掉头
        if(ch == ' ') {
            dir = -dir;
        }

        //加速
        if((ch == 'f' || ch == 'F') && (delay > 2)) {
            ndelay = delay / 2;
        }

        //减速
        if((ch == 's' || ch == 'S') && delay < 1000) {
            ndelay = delay * 2;
        }

        //更新计时器速度
        if(ndelay >= 1 && ndelay <= INT_MAX) {
            bounceSetTicker(delay = ndelay);
        }
    }
}

/**
 * 更新画面message
 **/
void bounceMoveMessage(int signum) {

    signal(SIGALRM, bounceMoveMessage);

    move(row, col);
    addstr(blank.c_str());
    
    col += dir;
    
    move(row, col);
    addstr(message.c_str());
    
    move(LINES - 1, 0);
    refresh();

    if(col + messageLen >= COLS) {
        dir = -1;
    }

    if(col <= 0) {
        dir = 1;
    }
}

/**
 * 设置信号时钟延迟单位毫秒
 **/
int bounceSetTicker(int delay) {
    struct itimerval timer;

    int n_sec = delay / 1000;
    int n_usec = (delay % 1000) * 1000;

    timer.it_value.tv_sec = n_sec;
    timer.it_value.tv_usec = n_usec;

    timer.it_interval.tv_sec = n_sec;
    timer.it_interval.tv_usec = n_usec;

    return setitimer(ITIMER_REAL, &timer, NULL);
}