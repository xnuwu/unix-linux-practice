#include "ls.hpp"

void listDirectory(const char* path)
{
    DIR* dirPtr;
    struct dirent* direntPtr;

    if((dirPtr = opendir(path)) == NULL) {
        std::cerr << "list directory " << path << " failed" << std::endl;
    }else{
        while((direntPtr = readdir(dirPtr)) != NULL) {
            std::cout << direntPtr -> d_name << std::endl;
        }
        closedir(dirPtr);
    }
}