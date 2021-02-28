#include "webserver/webserver.cpp"

int main(int argc, char* argv[]) {
    int sockFd = connectToServer("127.0.0.1", 6789);
    if(sockFd != -1) {
        FILE* sockInFp = fdopen(sockFd, "r");
        FILE* sockOutFp = fdopen(sockFd, "w");
        fputs("/home\r\n", sockOutFp);
        fflush(sockOutFp);

        char buf[256];
        while(fgets(buf, 256, sockInFp) != NULL) {
            std::cout << buf;
        }
        fclose(sockOutFp);
        fclose(sockInFp);
    }
}