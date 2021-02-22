#include <iostream>
#include <curses.h>
#include <unistd.h>
#include <stdlib.h>
#include "shell/smsh.cpp"
#include <fcntl.h>

int main(int argc, char* argv[]) {
    runShell();
}