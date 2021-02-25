#include "ticker_demo.hpp"
#include <iostream>
#include <cstdlib>
#include <sys/time.h>
#include <signal.h>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>

/**
 * 计时器打印计数
 **/
void countdown(int signal) {
    static int num = 10;
    std::cout << num-- << std::endl;
    if(num < 0) {
        std::cout << "done!" << std::endl;
        std::exit(0);
    }
}

/**
 * 设置定时器n_msec毫秒后开始定时执行
 **/
int setTicker(int n_msec) {
    struct itimerval new_timeset;
    long n_sec, n_usec;
    
    n_sec = n_msec / 1000; 
    n_usec = (n_msec % 1000) * 1000L; 

    new_timeset.it_value.tv_sec = n_sec;
    new_timeset.it_value.tv_usec = n_usec;

    new_timeset.it_interval.tv_sec = n_sec;
    new_timeset.it_interval.tv_usec = n_usec;

    return setitimer(ITIMER_REAL, &new_timeset, NULL);
}

/**
 * 信号处理器demo
 * 1、信号处理器处理后是否保持有效
 * 2、信号x到达后，信号y再到达会发生什么
 * 3、信号x到达处理中，信号x又到达会发生什么
 * 4、read阻塞时信号到达会发生什么
 **/
void signalDemo() {

    char input[SIGNAL_DEMO_INPUT_LEN];
    int nchars;

    signal(SIGINT, intHandler);
    signal(SIGQUIT, quitHandler);

    do {
        std::cout << "type a message" << std::endl;
        nchars = read(0, input, SIGNAL_DEMO_INPUT_LEN - 1);
        if(nchars == -1) {
            std::perror("read returned an error");
        }else{
            input[nchars] = '\0';
            std::cout << "You typed: " << input << std::endl;
        }
    } while(std::strncmp(input, "quit", 4) != 0);
}

/**
 * 中断信号处理
 **/
void intHandler(int signal) {
    std::cout << "Received signal " << signal << " waiting" << std::endl;
    sleep(2);
    std::cout << "Leaving intHandler" << std::endl;
}

/**
 * 退出信号处理
 **/
void quitHandler(int signal) {
    std::cout << "Received signal " << signal << " waiting" << std::endl;
    sleep(3);
    std::cout << "Leaving quitHandler" << std::endl;
}

/**
 * sigaction处理信号
 **/
void sigactionDemo() {
    
    struct sigaction newhandler;
    sigset_t blocked;
    char input[SIGNAL_DEMO_INPUT_LEN];
    int nchars;

    newhandler.sa_handler = intHandler;
    newhandler.sa_flags = SA_RESTART;

    sigemptyset(&blocked);
    sigaddset(&blocked, SIGQUIT);
    newhandler.sa_mask = blocked;

    if(sigaction(SIGINT, &newhandler, NULL) == -1) {
        std::perror("sigaction");
    }else{
        while(true) {
            nchars = read(0, input, SIGNAL_DEMO_INPUT_LEN - 1);
            if(nchars == -1) {
                std::perror("read returned an error");
            }else{
                input[nchars] = '\0';
                std::cout << "You typed: " << input << std::endl;
            }
        }
    }
}