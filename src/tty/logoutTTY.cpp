#include "logoutTTY.hpp"
#include "sys/types.h"
#include <ctime>
#include <unistd.h>
#include <error.h>
#include <cstdio>
#include <cstring>

int logoutTTY(const char* line)
{
    struct utmp record;
    int rw_fd;
    int utmpLen = sizeof(struct utmp);
    int readLen;

    if((rw_fd = open(UTMP_FILENAME, O_RDONLY)) == -1) {
        std::perror(UTMP_FILENAME);
    }

    int retval = -1;
    while((readLen = read(rw_fd, &record, utmpLen)) == utmpLen) {
        if(std::strncmp(record.ut_line, line, sizeof(record.ut_line)) == 0) {
            record.ut_type = DEAD_PROCESS;
            record.ut_time = time(nullptr);
            lseek(rw_fd, -utmpLen, SEEK_CUR);
            if(write(rw_fd, &record, utmpLen) == utmpLen) {
                retval = 0;
                break;
            }
        }
    }

    return retval;
}