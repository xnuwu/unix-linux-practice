#include "timeclnt.hpp"

int timeclnt(const char* host, const char* port) {
    if(host == NULL || port == NULL) {
        std::cerr << "host and port required" << std::endl;
        return 1;
    }
    struct sockaddr_in saddr;
    hostent* hp;
    int sockId;
    FILE* sockFd;
    char message[BUFF_SIZE];
    size_t messageLen;


    //创建socket
    sockId = socket(AF_INET, SOCK_STREAM, 0);
    if(sockId == -1) {
        oops("socket");
    }

    //获取远程服务地址端口
    memset(&saddr, 0, sizeof(saddr));
    hp = gethostbyname(host);
    if(hp == NULL) {
        oops(host);
    }

    bcopy(hp -> h_addr, (struct sockaddr *)&saddr.sin_addr, hp -> h_length);
    saddr.sin_port = htons(atoi(port));
    saddr.sin_family = AF_INET;

    //连接服务器
    if(connect(sockId, (struct sockaddr*) &saddr, sizeof(saddr)) != 0) {
        oops("connect");
    }

    //读取数据
    messageLen = read(sockId, message, BUFF_SIZE);
    if(messageLen > 0) {
        if(write(1, message, messageLen) != messageLen) {
            oops("write faield");
        }
    }
    close(sockId);
}