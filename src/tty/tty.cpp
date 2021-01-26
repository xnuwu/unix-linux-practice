#include "tty.hpp"
#include "sys/types.h"
#include <ctime>
#include <unistd.h>
#include <error.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>

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

void readTTYAndSave(const char* savePath)
{
    int out_fd, tty_fd;

    if((out_fd = creat(savePath,  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1) {
        std::perror(savePath);
        std::exit(-1);
    }

    if((tty_fd = open("/dev/tty", O_RDONLY)) == -1) {
        std::perror("open tty failed");
        std::exit(-1);
    }

    char* buffer[TTY_READ_BUFFER_SIZE];
    int n_chars;
    while(true) {
        if((n_chars = read(tty_fd, buffer, TTY_READ_BUFFER_SIZE)) > 0) {
            if(write(out_fd, buffer, n_chars) != n_chars) {
                std::perror("write file failed");
                exit(-1);
            }
        }else if(n_chars == -1){
            std::perror("read from tty failed");
            std::exit(-1);
        }
    }

    close(tty_fd);
    close(out_fd);
}