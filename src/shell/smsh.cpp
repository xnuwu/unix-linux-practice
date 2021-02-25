#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "unistd.h"
#include "sys/wait.h"
#include "signal.h"
#include "smsh.hpp"
#include "fcntl.h"

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
            ret = process(cmdList);
            freeList(cmdList);
        }
        free(cmd);
    }
}

/**
 * 设置环境
 **/
void setUp() {
    VEnviron2Table(environ);
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
        environ = VTable2environ();
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);

        char** ioRedirectPtr = checkIORedirect(cmd);
        if(ioRedirectPtr != NULL) {

            //替换标准输入、输出,然后从命令中去除对应的重定向参数
            if(**ioRedirectPtr == '>') {
                close(1);
                creat(*(ioRedirectPtr + 1), 0644);

                free(*ioRedirectPtr);
                free(*(ioRedirectPtr + 1));
                *ioRedirectPtr = NULL;
                *(ioRedirectPtr + 1) = NULL;

                execvp(cmd[0], cmd);
            }else if(**ioRedirectPtr == '<') {
                close(0);
                open(*(ioRedirectPtr + 1), O_RDONLY);

                free(*ioRedirectPtr);   
                free(*(ioRedirectPtr + 1));
                *ioRedirectPtr = NULL;
                *(ioRedirectPtr + 1) = NULL;

                execvp(cmd[0], cmd);
            }else {
                std::cerr << "no found io redirect flag" << std::endl;
            }
        }else{
            execvp(cmd[0], cmd);
        }
        
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

/**
 * 根据上下文条件处理当前行
 **/
int process(char* cmd[]) {
    
    if(cmd[0] == NULL) {
        return 0;
    }

    int rv = 0;

    if(isControlFlow(cmd[0])) {
        rv = doControlFlow(cmd);
    }else if(okToExecute()) {
        if(!builtInCommand(cmd, &rv)) {
            rv = execute(cmd);
        }
    }

    return rv;
}

/**
 * 判断当前行是否条件控制流代码
 **/
int isControlFlow(char* line) {
    return  strcmp("if", line) == 0 ||
            strcmp("then", line) == 0 ||
            strcmp("else", line) == 0 ||
            strcmp("fi", line) == 0;
}

/**
 * 执行控制流程主语句
 **/
int doControlFlow(char* cmd[]) {
    int rv = -1;
    if(strcmp("if", cmd[0]) == 0) {
        if(if_state != NEUTRAL) {
            rv = sysErr("'if' in wrong position");
        }else{
            last_stat = execute(cmd + 1);
            if_results = (last_stat == 0 ? SUCCESS : FAILED);
            if_state = WANT_THEN;
            rv = 0;
        }
    }else if(strcmp("then", cmd[0]) == 0) {
        if(if_state != WANT_THEN) {
            rv = sysErr("'then' in wrong positioin");
        }else{
            if_state = THEN_BLOCK;
            rv = 0;
        }
    }else if(strcmp("else", cmd[0]) == 0) {
        if(if_state != THEN_BLOCK) {
            rv = sysErr("'else' in wrong positioin");
        }else{
            if_state = ELSE_BLOCK;
            rv = 0;
        }
    }else if(strcmp("fi", cmd[0]) == 0) {
        if(if_state != ELSE_BLOCK) {
            rv = sysErr("'fi' in wrong positioin");
        }else{
            if_state = NEUTRAL;
            rv = 0;
        }
    }else{
        fatal("synatx error", cmd[0], 2);
    }

    return rv;
}

/**
 * 是否可以执行当前行代码
 **/
int okToExecute() {
    int rv = 0;

    if(if_state == NEUTRAL) {
        rv = 1;
    } else if(if_state == THEN_BLOCK) {
        if(if_results == SUCCESS) {
            rv = 1;
        }
    } else if(if_state == ELSE_BLOCK) {
        if(if_results == FAILED) {
            rv = 1;
        }
    } else {
        sysErr("then expected, please retry!");
    }

    return rv;
}

/**
 * 打印错误信息并返回
 **/
int sysErr(const char* msg) {
    if_state = NEUTRAL;
    std::cerr << "sysErr: " << msg << std::endl;
    return -1;
}

/**
 * 说明:判断是否包含builtIn命令参数,如果包含直接执行,并将执行结果赋值给resultPtr所指向的对象
 * 返回值: 1-builtIn命令,0-非builtIn命令
 **/
int builtInCommand(char* cmd[], int* resultPtr) {
    int rv = 0;
    
    if(strcmp(cmd[0], "set") == 0) {
        rv = 1;
        VList();
        *resultPtr = 0;
    } else if(strchr(cmd[0], '=') != NULL) {
        *resultPtr = assign(cmd[0]);
        if(*resultPtr != -1) {
            rv = 1;
        }
    } else if(strcmp(cmd[0], "export") == 0) {
        if(cmd[1] != NULL && okName(cmd[1]) == 0) {
            *resultPtr = VExport(cmd[1]);
        }else{
            *resultPtr = 1;
        }
        rv = 1;
    }

    return rv;
}

/**
 * 说明:进行环境变量赋值,cmd字符串格式为name=value
 * 返回值: 执行成功返回0,失败返回-1
 **/
int assign(char* cmd) {
    char* ptr = strchr(cmd, '=');
    *ptr = '\0';
    int rv = okName(cmd) == 0 ? VStore(cmd, ptr + 1) : -1;
    *ptr = '=';
    return rv;
}

/**
 * 说明:判断name是否符合命名规范
 * 返回值:0-符合规范,1-不符合规范
 **/
int okName(char* name) {

    char* ptr = name;
    int rv = 0;
    while(*ptr) {

        /**
         * 命名规则
         * 1、首字母不可为数字
         * 2、非首字母必须为字符、数字、下划线
         **/
        if((ptr == name && isdigit(*ptr)) || !(isalnum(*ptr) || *ptr == '_')) {
            rv = 1;
            break;
        }

        ptr++;
    }

    //字符串为空
    if(name == ptr) {
        rv = 1;
    }

    return rv;
}

/**
 * 说明:遍历打印变量列表
 **/
void VList() {
    for(int i = 0; i < MAX_VARS && environTab[i].str != NULL; i++) {
        if(environTab[i].global) {
            std::cout << "*" << environTab[i].str << std::endl;
        }else {
            std::cout << " " << environTab[i].str << std::endl;
        }
    }
}

/**
 * 说明:保存环境变量,如果已存在,则修改值。如果不存在则添加
 * 返回值:0-保存成功,1-保存失败
 **/
int VStore(const char* name, const char* value) {
    struct var* itemPtr;
    char* newEnvStrPtr;
    if((itemPtr = findItem(name, 1)) != NULL && (newEnvStrPtr = buildEnvStr(name, value)) != NULL) {
        //如果已存在,释放旧的条目值的内存
        if(itemPtr -> str != NULL) {
            free(itemPtr -> str);
        }
        itemPtr -> str = newEnvStrPtr;
    }

    return 0;
}

/**
 * 说明:构造环境变量字符串
 * 返回值:返回环境变量字符串指针
 **/
char* buildEnvStr(const char* name, const char* value) {
    char* rv;
    rv = (char*) emalloc(strlen(name) + strlen(value) + 2);
    if(rv == NULL) {
        return NULL;
    }
    sprintf(rv, "%s=%s", name, value);
    return rv;
}

/**
 * 说明:在列表中查找name对应的条目
 * 返回值:如果找到返回对应的指针。未找到根据firsBlank返回（0-返回NULL,1-返回首个空条目对应的指针)
 **/
static struct var* findItem(const char* name, int firstBlank) {

    int i = 0;
    for(; i < MAX_VARS && environTab[i].str != NULL; i++) {
        if(strncmp(name, environTab[i].str, strlen(name)) == 0 && environTab[i].str[strlen(name)] == '=') {
            return &environTab[i];
        }
    }

    if(i == MAX_VARS || !firstBlank) {
        return NULL;
    }
    return &environTab[i];
}

/**
 * 说明:将变量输出到环境变量中
 * 返回值:0-成功,1-失败
 **/
int VExport(const char* name) {
    int rv = 0;
    struct var* itemPtr;
    if((itemPtr = findItem(name, 0)) != NULL) {
        itemPtr -> global = 1;
        rv = 0;
    }else if(VStore(name, "") != 1){
        rv = VExport(name);
    }

    return rv;
}

/**
 * 说明:查找name对应的环境变量
 * 返回值:如果找到返回对应的字符串指针,未找到返回NULL
 **/
char* VLookUp(const char* name) {
    struct var* itemPtr;
    if((itemPtr = findItem(name, 0)) != NULL) {
        return itemPtr -> str;
    }
    return NULL;
}

/**
 * 说明:将当前环境变量保存到变量表中
 * 返回值:0-成功,1-失败
 **/
int VEnviron2Table(char* env[]) {
    struct var* itemPtr;
    int i;
    for(i = 0; i < MAX_VARS && env[i] != NULL; i++) {
        char* strPtr = (char*) emalloc(strlen(env[i]) + 1);
        if(strPtr == NULL) {
            return 1;
        }
        environTab[i].str = strcpy(strPtr, environ[i]);
        environTab[i].global = 1;
    }

    while(i < MAX_VARS) {
        environTab[i].str = NULL;
        environTab[i++].global = 0;
    }
    return 1;
}

/**
 * 说明:将变量表保存到当前环境变量中
 * 返回值:返回当前环境变量指针
 **/
char** VTable2environ() {
    int i, j, n = 0;
    char** envtab;

    for(i = 0; i < MAX_VARS && environTab[i].str != NULL; i++) {
        if(environTab[i].global) {
            n++;
        }
    }

    envtab = (char**) emalloc(sizeof(char*) * (n + 1));
    if(envtab == NULL) {
        return NULL;
    }

    for(int i = 0, j = 0; i < MAX_VARS && environTab[i].str != NULL; i++) {
        if(environTab[i].global == 1) {
            envtab[j++] = environTab[i].str;
        }
    }
    envtab[j] = NULL;

    return envtab;
}

/**
 * 说明:检查命令是否包含io重定向,重定向符号前后必须有空格
 * 返回:如果包含重定向，返回指向io重定向符号的指针，否则返回NULL
 **/
char** checkIORedirect(char* cmd[]) {
    if(cmd[0] == NULL) {
        return NULL;
    }

    for(int i = 0; cmd[i] != NULL; i++) {
        if(strchr(cmd[i], '<') != NULL || strchr(cmd[i], '>') != NULL) {
            return &cmd[i];
        }
    }

    return NULL;
}

