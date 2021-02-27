#include <iostream>
#include <curses.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "shell/pipe.cpp"
#include "shell/timeserv.cpp"

int main(int argc, char* argv[]) {
    timeserv();
}