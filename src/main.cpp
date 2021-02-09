#include <iostream>
#include <curses.h>
#include <unistd.h>
#include <signal.h>
#include "game/game.cpp"
#include "game/ticker_demo.cpp"
#include "game/bounceld.cpp"

int main(int argc, char* argv[]) {
    bounce("Xunwu & Lily");
}