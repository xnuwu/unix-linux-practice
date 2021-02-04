#include "tty.hpp"
#include "sys/types.h"
#include <ctime>
#include <unistd.h>
#include <error.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>

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

void readTTYAndSaveToFile(const char* savePath)
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

bool echoState() {
    struct termios info;
    int rv;

    //0代表stdin
    rv = tcgetattr(0, &info);
    if(rv == -1) {
        std::perror("tcgetattr");
        std::exit(-1);
    }

    return info.c_lflag & ECHO;
}

bool setEchoState(bool state) {
    struct termios info;
    int rv;
    
    //0代表stdin
    if(tcgetattr(0, &info) == -1) {
        std::perror("tcgetattr");
        std::exit(-1);
    }

    if(state) {
        info.c_lflag |= ECHO;
    } else {
        info.c_lflag &= ~ECHO;
    }

    return info.c_lflag & ECHO;
}

void showTTYInfo() {
    struct termios ttyInfo;
    if(tcgetattr(0, &ttyInfo) == -1) {
        std::perror("tcgetattr");
        std::exit(-1);
    }

    showBaud(cfgetospeed(&ttyInfo));
    std::cout << "the erase character is ascii " << ttyInfo.c_cc[VERASE] << ", Ctrl-" << 'A' + (ttyInfo.c_cc[VERASE] - 1) << std::endl;
    std::cout << "the line kill character is ascii " << ttyInfo.c_cc[VKILL] << ", Ctrl-" << 'A' + (ttyInfo.c_cc[VKILL] - 1) << std::endl;

    showTTYFlagInfo(&ttyInfo);
}

void showBaud(speed_t baudSpeed) {
    std::cout << "the baud rate is ";
    switch(baudSpeed) {
        case B300: std::cout << "B300" << std::endl;
        case B600: std::cout << "B600" << std::endl;
        case B1200: std::cout << "B1200" << std::endl;
        case B1800: std::cout << "B1800" << std::endl;
        case B2400: std::cout << "B2400" << std::endl;
        case B4800: std::cout << "B4800" << std::endl;
        case B9600: std::cout << "B9600" << std::endl;
        default:
            std::cout << "fast" << std::endl;
    }
}

struct flagInfo {
    int fl_value;
    const char* fl_name;
};

struct flagInfo input_flags[] = {
    IGNBRK, "Ignore break condition",
    BRKINT, "Signal interrupt on break",
    IGNPAR, "Ignore char with parity errors",
    PARMRK, "Mark parity errors",
    0, NULL
};

struct flagInfo local_flags[] = {
    INPCK, "Enable input parity check",
    ISTRIP, "Strip character",
    INLCR, "Map NL to CR on Input",
    ECHO, "Enable echo",
    0, NULL
};

void showTTYFlagInfo(struct termios* ttyInfoPtr) {
    showFlagSet(ttyInfoPtr -> c_iflag, input_flags);
    showFlagSet(ttyInfoPtr -> c_lflag, local_flags);
}

void showFlagSet(int flagValue, struct flagInfo theBitName[]) 
{
    for(int i = 0; theBitName[i].fl_value; i++) {
        std::cout << theBitName[i].fl_name << " is " << ((theBitName[i].fl_value & flagValue) ? "ON" : "OFF") << std::endl;
    }
}