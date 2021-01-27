#include "ls.hpp"
#include <stdlib.h>
#include <cstring>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <string>
#include <iomanip>
#include <time.h>
#include <cstdio>

//打印目录
void listDirectory(const char *path, const listDirectoryOptions options) {

    DIR *dirPtr;
    struct dirent *direntPtr;
    struct stat info;
    std::string basePath(path);

    if ((dirPtr = opendir(path)) == NULL) {
        std::cerr << "list directory " << path << " failed" << std::endl;
    } else {
        
        while ((direntPtr = readdir(dirPtr)) != NULL) {
            
            if (options.displayAll || direntPtr->d_name[0] != '.') {
                std::string fullPath(basePath);
                fullPath.append("/").append(direntPtr -> d_name);

                if(stat(fullPath.c_str(), &info) != -1) {
                    showFileDetailInfo(direntPtr -> d_name, info);
                }else{
                    std::perror(direntPtr -> d_name);
                }
            }
        }
        closedir(dirPtr);
    }
    std::cout << std::endl;
}

//显示文件详细信息
void showFileDetailInfo(const char* filename, struct stat info) {
    
    char modeStr[11] = { 0 };
    modeToLetters(info.st_mode, modeStr);

    const char* ctimeStrBuffer;
    char timeStrBuffer[50];
    std::cout << modeStr;
    std::cout << std::setw(4) << info.st_nlink;
    std::cout << std::setw(8) << std::right << getNameByUID(info.st_uid);
    std::cout << std::setw(8) << std::right << getGroupNameByGID(info.st_gid);
    std::cout << std::setw(8) << std::right << info.st_size;

    ctimeStrBuffer = ctime(&info.st_mtime);
    strncpy(timeStrBuffer, ctimeStrBuffer, strlen(ctimeStrBuffer) - 1);
    timeStrBuffer[strlen(ctimeStrBuffer) - 1] = '\0';

    std::cout << "  " << std::right << timeStrBuffer;
    std::cout << "  " << filename;
    std::cout << std::endl;
}

//打印属性权限
void modeToLetters(int mode, char str[]) {
    strcpy(str, "----------");
    if(S_ISDIR(mode)) { str[0] = 'd'; }
    if(S_ISCHR(mode)) { str[0] = 'c'; }
    if(S_ISBLK(mode)) { str[0] = 'b'; }

    if(mode & S_IRUSR) { str[1] = 'r'; }
    if(mode & S_IWUSR) { str[2] = 'w'; }
    if(mode & S_IXUSR) { str[3] = 'x'; }

    if(mode & S_IRGRP) { str[4] = 'r'; }
    if(mode & S_IWGRP) { str[5] = 'w'; }
    if(mode & S_IXGRP) { str[6] = 'x'; }

    if(mode & S_IROTH) { str[7] = 'r'; }
    if(mode & S_IWOTH) { str[8] = 'w'; }
    if(mode & S_IXOTH) { str[9] = 'x'; }
}

//根据UID获取用户姓名
const char* getNameByUID(uid_t uid) {
    struct passwd* pwPtr;
    static char uidStr[10];
    if((pwPtr = getpwuid(uid)) == NULL) {
        std::sprintf(uidStr, "%d", uid);
        return uidStr;
    }

    return pwPtr -> pw_name;
}

//根据组GID获取组名称
const char* getGroupNameByGID(gid_t gid) {
    struct group* gpPtr;
    static char gidStr[10];
    if((gpPtr = getgrgid(gid)) == NULL) {
        sprintf(gidStr, "%d", gid);
        return gidStr;
    }

    return gpPtr -> gr_name;
}
