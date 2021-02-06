#include <iostream>
#include <curses.h>
#include <unistd.h>

int main(int argc, char* argv[]) {

    initscr();
    clear();
    
    for(int i = 0; i < LINES; i++) {
        move(i, i + 1);
        if(i % 2 == 1) {
            standout();
        }
        addstr("Hello,World");
        if(i % 2 == 1) {
            standend();
        }
        sleep(1);
        refresh();  
    }
    endwin();
}