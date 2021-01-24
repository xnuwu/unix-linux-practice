#define DEBUG_LOG

#include "who/who.cpp"
#include <cstdio>
#include <cstdlib>

int main(int argc, char* argv[])
{
    struct utmp* record;
    
    if(utmpOpen(UTMP_FILENAME) == -1) {
        std::perror(UTMP_FILENAME);
        std::exit(-1);
    }

    while((record = utmpNext()) != NULLUT) {
        showInfo(record);
    }

    utmpClose();
    return 0;
}