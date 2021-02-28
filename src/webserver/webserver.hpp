#ifndef WEBSERVER
#define WEBSERVER

#define oops(msg) {std::perror(msg);}
#define HOST_LEN 256

int makeServerSocket(int port);
int makeServerSocketQ(int port, int backlog);
int connectToServer(char* host, int port);

#endif