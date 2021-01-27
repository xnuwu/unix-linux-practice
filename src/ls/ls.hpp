#ifndef LS
#define LS

#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <dirent.h>
#include <iostream>
#include <unistd.h>

typedef struct listDirectoryOptions {
    bool displayAll = false;
    bool displayDetail = false;
} listDirectoryOptions;

//打印目录
void listDirectory(const char* path, const listDirectoryOptions options);

//显示文件详细信息
void showFileDetailInfo(const char* filename, struct stat info);

//打印属性权限
void modeToLetters(int mode, char str[]);

//根据UID获取用户姓名
const char* getNameByUID(uid_t uid);

//根据组GID获取组名称
const char* getGroupNameByGID(gid_t gid);

#endif