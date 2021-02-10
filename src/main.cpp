#include <iostream>
#include <curses.h>
#include <unistd.h>
#include <signal.h>
#include "game/bounce2d.cpp"

int main(int argc, char* argv[]) {
    playBounce2D();
}