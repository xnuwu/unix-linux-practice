#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "unistd.h"
#include "sys/wait.h"
#include "signal.h"
#include "smsh.hpp"

/**
 * 运行shell
 **/
void runShell() {
    char*  cmd;
    char** cmdList;
    const char*  prompt = DEL_PROMPT;
    int ret;
    setUp();

    while((cmd = readCommand(prompt, stdin)) != NULL) {
        if((cmdList = splitLine(cmd)) != NULL) {
            ret = execute(cmdList);
            freeList(cmdList);
        }
        free(cmd);
    }
}

/**
 * 设置环境
 **/
void setUp() {
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

/**
 * 从用户输入读取命令
 **/
char* readCommand(const char* prompt, FILE* fp) {

#ifdef DEBUG
    std::cout << "readCommand() start" << std::endl;
#endif 

    std::cout << prompt;
    size_t bulkSize = 32;
    char* buffer = NULL;
    size_t bufferSize = 0;
    char ch;
    size_t len = 0;

    while((ch = getc(fp)) != EOF) {
        if(len + 1 >= bufferSize) {
            if(buffer) {
                buffer = (char*) erealloc(buffer, bufferSize + bulkSize);
            }else{
                buffer = (char*) emalloc(bulkSize);
            }
            bufferSize += bulkSize;
        }

        if(ch == '\n') {
            break;
        }

        buffer[len++] = ch;
    }

    if(ch == EOF && len == 0) {
        return NULL;
    }

    buffer[len] = '\0';

#ifdef DEBUG
    std::cout << "readCommand() end " << std::endl;
#endif 

    return buffer;
}

/**
 * 按照空格分隔命令
 **/
char** splitLine(char* line) {
    
#ifdef DEBUG
    std::cout << "splitLine(" << line << ") start" << std::endl;
#endif 

    if(line == NULL) {
        std::cout << "line is NULL" << std::endl;
        return NULL;
    }

    char** list = (char**) emalloc(BUFFER_SIZE);
    size_t bufferSize = BUFFER_SIZE / sizeof(char*);
    size_t bufferSpace = BUFFER_SIZE;
    size_t argnum = 0;
    size_t len;
    char* start;
    char* cp = line;

    while(*cp != '\0') {
        while(IS_DELIME(*cp)) {
            cp++;
        }

        if(*cp == '\0') {
            break;
        }

        if(argnum + 1 >= bufferSize) {
            list = (char**) erealloc(list, bufferSpace + BUFFER_SIZE);
            bufferSpace += BUFFER_SIZE;
            bufferSize += BUFFER_SIZE / sizeof(char*);
        }

        start = cp;
        len = 1;
        while(*++cp != '\0' && !(IS_DELIME(*cp))) {
            len++;
        }
        list[argnum++] = newstr(start, len);
    }

    list[argnum] = NULL;

#ifdef DEBUG
    std::cout << "splitLine() end" << std::endl;
#endif 

    return list;
}

/**
 * 重新复制分配字符串
 **/
char* newstr(char* cp, size_t len) {

#ifdef DEBUG
    std::cout << "newstr " << cp << " " << len << " start " << std::endl;
#endif

    char* str = (char*) emalloc(len + 1);
    str[len] = '\0';
    strncpy(str, cp, len);

#ifdef DEBUG
    std::cout << "newstr " << cp << " " << len << " end " << str << std::endl;
#endif 

    return str;
}

/**
 * 释放字符串数字
 **/
char* freeList(char** list) {
    char** cp = list;
    while(*cp) {
        free(*cp++);
    }
    free(list);
}

/**
 * 分配内存
 **/
void* emalloc(size_t n) {
    void* rv = malloc(n);
    if(rv == NULL) {
        fatal("malloc failed", "", 1);
    }
    return rv;
}

/**
 * 重新分配内存
 **/
void* erealloc(void* p, size_t n) {
    void* rv;
    if((rv = realloc(p, n)) == NULL) {
        fatal("realloc failed", "", 1);
    }
    return rv;
}

/**
 * execvp执行cmd命令
 **/
int execute(char* cmd[]) {

#ifdef DEBUG
    std::cout << "execute() start" << std::endl;
#endif 

    int pid;
    int childInfo = -1;
    
    if(cmd[0] == NULL) {
        return 0;
    }

    if((pid = fork()) == -1) {
        perror("fork");
    }else if(pid == 0) {
        //子进程
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        execvp(cmd[0], cmd);

#ifdef DEBUG
    std::cout << "child execute() end" << std::endl;
#endif 

        perror("cannot execvp command");
        exit(1);
    }else {
        //父进程
        if(wait(&childInfo) == -1) {
            perror("wait");
        }

#ifdef DEBUG
    std::cout << "parent execute() end" << std::endl;
#endif 
        return childInfo;
    }
}

/**
 * 错误消息打印
 **/
void fatal(const char* label,const char* detail, int rv) {
    std::cerr << "Error: " << label << ", " << detail << std::endl;
}
