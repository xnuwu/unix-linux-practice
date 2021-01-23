#include "who/who.cpp"
#include <cstdio>
#include <cstdlib>

int main(int argc, char* argv[])
{
    utmp currentRecord;
    int utmpfd;
    int recordLen = sizeof(currentRecord);

    if((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1) {
        std::perror(UTMP_FILE);
        std::exit(1);
    }

    while(read(utmpfd, &currentRecord, recordLen) == recordLen) {
        showInfo(currentRecord);
    }

    close(utmpfd);
    return 0;
}