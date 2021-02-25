#ifndef PWD
#define PWD

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <dirent.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iomanip>

#define BUFFER_SIZE 256

//打印i-节点路径
void printCurrentPath(ino_t inode);

//获取指定i-节点的名称
void getINodeName(ino_t inode, char* name, int bufferSize);

//获取指定目录文件的i-节点
ino_t getINode(const char* pathname);

#endif