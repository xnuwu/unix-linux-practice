#ifndef BOUNCE_2D
#define BOUNCE_2D

#include <string>

#define LINES 30
#define COLS 100

#define BLANK ' '
#define DEL_SYMBOL 'o'
#define TOP_ROW 1
#define BOT_ROW (LINES -1)
#define LEFT_EDGE 1
#define RIGHT_EDGE (COLS - 1)
#define X_INIT 1
#define Y_INIT 1
#define TICKS_PER_SEC   15
#define Y_TTM 4
#define X_TTM 2

#define SLIDE_SYMBOL "####################"
#define SLIDE_BLANK  "                    "
#define SLIDE_LEFT_Y_POS (LINES - 5)
#define SLIDE_LEFT_X_POS 1
#define SLIDE_TTM 2

struct Slide {
    int y_pos, x_pos;   //滑板最左侧坐标
    int dir;            //方向1向右，-1向左
    int ttm, ttg;       //时钟更新速度
    std::string symbol; //滑板图标
    std::string blank;  //清空滑板
};

struct Ball {
    int y_pos, x_pos,   //球坐标
    y_ttm, x_ttm,       //移动速率
    y_ttg, x_ttg,       //当前剩余信号数量
    y_dir, x_dir;       //移动方向,y:1向右,y:-1向左,x:1向下,x:-1向上
    char symbol;        //球图标
};

void setUp();
void wrapUp();
void ballAndSlideMove(int signum);
int bounceOrLose(struct Ball* ballPtr);
void playBounce2D();
int setTicker(int msecs);


#endif