#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

/**
 * 处理signal信号
 **/
void signalHandler(int signal) {
    std::cout << "signalHandler get signlal " << signal << std::endl;
}

int main(int argc, char* argv[]) {

    /**
     * kill -9
     **/
    signal(SIGKILL, signalHandler); 
    
    /**
     * ctrl + z
     **/
    signal(SIGSTOP, signalHandler);

    
    int fd = open("/home/course/unix/output.data", O_RDWR | O_TRUNC | O_CREAT);
    if(fd == -1) {
        std::perror("/home/course/unix/output.data");
        exit(1);
    }
    while(true) {
        sleep(1);
        write(fd, "abc\n", 4);
    }
}