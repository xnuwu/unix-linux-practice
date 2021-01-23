#include "who.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <ctime>
#include <iostream>
#include <iomanip>

void showInfo(const utmp& utmpR) 
{
    if(utmpR.ut_type != USER_PROCESS) {
        std::cout << std::setw(12) << std::left << utmpR.ut_name;
        std::cout << std::setw(8) << std::left << utmpR.ut_line;
        showTime(utmpR.ut_time);
#ifdef SHOWHOST
    if(utmpR.ut_host[0] != '\0') {
        std::cout << std::setw(12) << std::left << utmpR.ut_host;
    }
#endif
    std::cout << std::endl;
    }
}

void showTime(long timeval) 
{
    char timeBuffer[50];
    std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", std::localtime(&timeval));
    std::cout << std::setw(24) << std::left << timeBuffer;
}