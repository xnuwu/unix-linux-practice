#include "pwd.hpp"

//打印i-节点路径
void printCurrentPath(ino_t inode) {

    char buffer[BUFFER_SIZE];
    ino_t myINode;
    if(getINode("..") != inode) {
        chdir("..");
        getINodeName(inode, buffer, BUFFER_SIZE);
        myINode = getINode(".");
        printCurrentPath(myINode);
        std::cout << "/" << buffer;
    }
}

//获取指定i-节点的名称
void getINodeName(ino_t inodeToFind, char* name, int bufferSize) {
    
    DIR* dirPtr;
    struct dirent *direntPtr;
    dirPtr = opendir(".");
    if(dirPtr == NULL) {
        std::perror(".");
        std::exit(-1);
    }

    while((direntPtr = readdir(dirPtr)) != NULL) {
        if(direntPtr -> d_ino == inodeToFind) {
            std::strncpy(name, direntPtr -> d_name, bufferSize);
            name[bufferSize - 1] = '\0';
            closedir(dirPtr);
            return;
        }
    }
    std::cerr << "error looking for inode " << inodeToFind << std::endl;
    std::exit(-1);
}

//获取指定目录文件的i-节点
ino_t getINode(const char* pathname) {

    struct stat info;
    if(stat(pathname, &info) == -1) {
        std::perror(pathname);
        std::exit(-1);
    }

    return info.st_ino;
}