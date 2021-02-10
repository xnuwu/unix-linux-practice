#include "bounce2d.hpp"
#include "signal.h"
#include <sys/time.h>
#include <curses.h>
#include <climits>
#include <string>

struct Ball theBall;
struct Slide theSlide;
int isFailed = 0;

/**
 * 初始化游戏,设置屏幕,球位置,滑块位置
 **/
void setUp() {

    theBall.x_ttm = theBall.x_ttg = X_TTM;
    theBall.y_ttm = theBall.y_ttg = Y_TTM;
    theBall.x_pos = X_INIT;
    theBall.y_pos = Y_INIT;
    theBall.x_dir = 1;
    theBall.y_dir = 1;
    theBall.symbol = 'o';

    theSlide.x_pos = SLIDE_LEFT_X_POS;
    theSlide.y_pos = SLIDE_LEFT_Y_POS;
    theSlide.symbol = SLIDE_SYMBOL;
    theSlide.blank = SLIDE_BLANK;
    theSlide.dir = 1;
    theSlide.ttm = SLIDE_TTM;
    theSlide.ttg = SLIDE_TTM;

    isFailed = 0;

    initscr();
    clear();
    noecho();
    crmode();
    
    signal(SIGINT, SIG_IGN);
    mvaddch(Y_INIT, X_INIT, DEL_SYMBOL);
    move(theSlide.y_pos, theSlide.x_pos);
    addstr(theSlide.symbol.c_str());
    refresh();

    signal(SIGALRM, ballAndSlideMove);
    setTicker(TICKS_PER_SEC);
}

/**
 * 结束游戏,重置屏幕
 **/
void wrapUp() {
    setTicker(0);
    endwin();
}

/**
 * 移动球,滑板
 **/
void ballAndSlideMove(int signum) {

    signal(SIGALRM, SIG_IGN);

    //记录当前球坐标用于后面清屏
    int x_cur = theBall.x_pos;
    int y_cur = theBall.y_pos;
    int moved = 0;

    //移动x方向
    if(theBall.x_ttm > 0 && theBall.x_ttg-- == 1) {
        theBall.x_ttg = theBall.x_ttm;
        theBall.x_pos += theBall.x_dir;
        moved = 1;
    }

    //移动y方向
    if(theBall.y_ttm > 0 && theBall.y_ttg-- == 1) {
        theBall.y_ttg = theBall.y_ttm;
        theBall.y_pos += theBall.y_dir;
        moved = 1;
    }

    //移动滑板
    if(theSlide.ttg-- == 1) {
        move(theSlide.y_pos, theSlide.x_pos);
        addstr(theSlide.blank.c_str());
        
        theSlide.x_pos += theSlide.dir;
        
        move(theSlide.y_pos, theSlide.x_pos);
        addstr(theSlide.symbol.c_str());

        if(theSlide.x_pos + theSlide.symbol.length() >= COLS) {
            theSlide.dir = -1;
        }

        if(theSlide.x_pos <= 0) {
            theSlide.dir = 1;
        }
        theSlide.ttg = theSlide.ttm;
    }

    if(moved) {
        mvaddch(y_cur, x_cur, BLANK);
        mvaddch(theBall.y_pos, theBall.x_pos, DEL_SYMBOL);
        if(bounceOrLose(&theBall)) {
            setTicker(0);
            move(LINES / 2, (COLS - theSlide.symbol.length()) / 2);
            addstr("You Failed! Restart Press [R]");
            isFailed = 1;
        }
    }

    move(LINES - 1, COLS - 1);
    refresh();
    signal(SIGALRM, ballAndSlideMove);
}

/**
 * 更新当前球的移动方向,判断游戏是否结束1,未结束0
 **/
int bounceOrLose(struct Ball* ballPtr) {
    int isLose = 0;
    if(ballPtr -> x_pos == LEFT_EDGE) {
        ballPtr -> x_dir = 1;
    }

    if(ballPtr -> x_pos == RIGHT_EDGE) {
        ballPtr -> x_dir = -1;
    }

    if(ballPtr -> y_pos == TOP_ROW) {
        ballPtr -> y_dir = 1;
    }

    if(ballPtr -> y_pos == BOT_ROW) {
        ballPtr -> y_dir = -1;
    }

    //当球到达滑板所在水平线判断是否"接住"
    if(ballPtr -> y_pos == theSlide.y_pos - 1) {
        if(ballPtr -> x_pos >= theSlide.x_pos && ballPtr -> x_pos <= theSlide.x_pos + theSlide.symbol.length()) {
            ballPtr -> y_dir = -1;
            isLose = 0;
        }else{
            isLose = 1;
        }
    }

    return isLose;
}

/**
 * 设置毫秒定时器
 **/
int setTicker(int msecs) {
    struct itimerval time_set;

    int n_secs = msecs / 1000;
    int n_usecs = (msecs % 1000) * 1000;

    time_set.it_value.tv_sec = n_secs;
    time_set.it_value.tv_usec = n_usecs;

    time_set.it_interval.tv_sec = n_secs;
    time_set.it_interval.tv_usec = n_usecs;

    return setitimer(ITIMER_REAL, &time_set, NULL);
}

/**
 * 启动游戏
 **/
void playBounce2D() {
    
    int ch;
    setUp();

    while((ch = getch()) && ch != 'Q' && ch != 'q') {
        if(ch == 's' && theBall.x_ttm < INT_MAX) { theBall.x_ttm++;}
        if(ch == 'S' && theBall.y_ttm < INT_MAX) { theBall.y_ttm++;}
        if(ch == 'f' && theBall.x_ttm > 1) { theBall.x_ttm--;}
        if(ch == 'F' && theBall.y_ttm > 1) { theBall.y_ttm--;}
        if((ch == 'a' || ch == 'A') && theSlide.ttm < INT_MAX) { theSlide.ttm *= 2;}
        if((ch == 'd' || ch == 'D') && theSlide.ttm > 1) { theSlide.ttm /= 2;}
        if(ch == ' ') { theSlide.dir = -theSlide.dir; }
        if((ch == 'r' || ch == 'R') && isFailed) { wrapUp(); setUp(); }
    }

    wrapUp();
}