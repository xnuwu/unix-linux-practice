#include "who.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <ctime>
#include <iostream>
#include <iomanip>

//显示单条utmp记录数据
void showInfo(struct utmp* utmpR) 
{
    if(utmpR -> ut_type != USER_PROCESS) {
        std::cout << std::setw(12) << std::left << utmpR -> ut_name;
        std::cout << std::setw(8) << std::left << utmpR -> ut_line;
        showTime(utmpR -> ut_time);
        
#ifdef SHOWHOST
        if(utmpR -> ut_host[0] != '\0') {
            std::cout << std::setw(12) << std::left << utmpR -> ut_host;
        }
#endif

    std::cout << std::endl;
    } else{
        
#ifdef DEBUG_LOG
    std::cout << "utmp ut_type is USER_PROCESS(" << USER_PROCESS << ")" << std::endl;
#endif

    }
}

//格式化显示时间
void showTime(long timeval) 
{
    char timeBuffer[50];
    std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", std::localtime(&timeval));
    std::cout << std::setw(24) << std::left << timeBuffer;
}

//打开utmp文件
int utmpOpen(const char* utmpPath)
{
    if((utmp_fd = open(utmpPath, O_RDONLY)) == -1) {
        std::perror(utmpPath);
        exit(-1);
    }

    utmp_total_buffer_num = 0;
    utmp_current_index = 0;

    return utmp_fd;
}

//读取并缓存utmp数据
struct utmp* utmpNext() 
{
    if(utmp_current_index == utmp_total_buffer_num && utmpLoad() == 0) {
        return NULLUT;
    }

#ifdef DEBUG_LOG
    std::cout << "current utmp index " << utmp_current_index << std::endl;
#endif

    struct utmp* ret = (struct utmp*)(&utmp_buffer[utmp_current_index * UTSIZE]);
    utmp_current_index++;
    return ret;
}

//批量加载utmp数据，返回加载的成功数量
int utmpLoad() 
{
    int n_chars = 0;
    if((n_chars = read(utmp_fd, utmp_buffer, UTNUM * UTSIZE)) == -1) {
        return 0;
    }

    utmp_current_index = 0;
    utmp_total_buffer_num = n_chars / UTSIZE;

#ifdef DEBUG_LOG
    std::cout << "total load " << utmp_total_buffer_num << " utmp" << std::endl;
#endif

    return utmp_total_buffer_num;
}

//关闭utmp文件并清理缓存
void utmpClose() 
{
    if(utmp_fd != -1) {
        close(utmp_fd);
    }
}