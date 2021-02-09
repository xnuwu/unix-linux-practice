#ifndef GAME
#define GAME

#include <string>

#define LEFT_EDGE 10
#define RIGHT_EDGE (COLS - 20)
#define ROW (LINES / 2)

/**
 * 字符串弹来弹去
 **/
void wordPlayAround(const std::string& word);

#endif