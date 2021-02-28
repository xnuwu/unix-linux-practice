#include "timeclnt2.hpp"
#include "socketlib.cpp"

/**
 * description: connect to server and get server's time
 **/
void timeclnt2() {
    int sockFd = connectToServer("127.0.0.1", PORTNUM);
    if(sockFd == -1) {
        std::cerr << "connect to server faild" << std::endl;
        return;
    }
    talkWithServer(sockFd);
}

void talkWithServer(int sockFd) {
    char buf[BUFF_SIZE];
    size_t readNum;
    while((readNum = read(sockFd, buf, BUFF_SIZE)) > 0) {
        if(write(1, buf, readNum) != readNum) {
            oops("write");
        }
    }
    close(sockFd);
}