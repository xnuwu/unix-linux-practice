#include "game.hpp"
#include <curses.h>
#include <string>
#include <unistd.h>

/**
 * 字符串弹来弹去
 **/
void wordPlayAround(const std::string& word) {

    size_t wordLength = word.length();
    std::string blankString(wordLength, ' ');

    initscr();
    clear();

    int dir = 1;
    int pos = LEFT_EDGE;

    while(true) {
        move(ROW, pos);
        addstr(word.c_str());
        move(LINES - 1, COLS - 1);
        refresh();
        usleep(50000);
        move(ROW, pos);
        addstr(blankString.c_str());
        pos += dir;
        if(pos <= LEFT_EDGE) {
            dir = 1;
        }else if(pos >= RIGHT_EDGE) {
            dir = -1;
        }
    }
}