#include "unistd.h"
#include "fcntl.h"
#include "tinybc.hpp"
#include "string.h"
#include "stdio.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "wait.h"

/**
 * 说明:bc计算器实现
 **/
void tinybc() {

    int toDC[2], fromDC[2];

    if(pipe(toDC) == -1 || pipe(fromDC) == -1) {
        std::perror("pipe open failed");
        std::exit(1);
    }

    int pid = fork();
    if(pid == -1) {
        std::perror("fork failed");
        std::exit(1);
    }else if(pid == 0) {
        dc(toDC, fromDC);
    }else {
        bc(toDC, fromDC);
        wait(NULL);
    }
}

/**
 * 说明:求值表达式转换为逆波兰表达式后,通过管道转发给dc,并获取bc的执行标准输出
 **/
void bc(int toDC[2], int fromDC[2]) {

    int num1, num2;
    size_t exp_size;
    char exp[BUFF_SIZE];
    char result[BUFF_SIZE];
    char op[OPERATOR_BUFF_SIZE];

    close(toDC[0]);
    close(fromDC[1]);

    FILE* output = fdopen(toDC[1], "w");
    FILE* input = fdopen(fromDC[0], "r");

    while(std::cout << "tinybc:" && fgets(exp, BUFF_SIZE, stdin) != NULL) {

        if(sscanf(exp, "%d %[+-*/^] %d", &num1, op, &num2) != 3) {
            std::cerr << "wrong format input, eg: 1 + 2" << std::endl;
            continue;
        }

        if(fprintf(output, "%d\n %d\n%c\np\n", num1, num2, *op) == EOF) {
            std::cerr << "error writing to dc" << std::endl;
            continue;
        }
        
        fflush(output);
        if(fgets(result, BUFF_SIZE, input) != NULL) {
            std::cout << num1 << " " << op << " " << num2 << " = " << result;
        }else{
            std::cerr << "read result failed" << std::endl;
            break;
        }
    }

    fclose(input);
    fclose(output);
}

/**
 * 说明:dc进程获取管道输入的逆波兰表达式求值后,通过管道将标准输出返回给bc
 **/
void dc(int toDC[2], int fromDC[2]) {

    if(dup2(toDC[0], 0) == -1) {
        std::perror("dup2 toDC[1] failed");
        std::exit(1);
    }
    close(toDC[0]);
    close(toDC[1]);

    if(dup2(fromDC[1], 1) == -1) {
        std::perror("dup2 fromDC[1] failed");
        std::exit(1);
    }
    close(fromDC[0]);
    close(fromDC[1]);

    execlp("dc", "dc", "-", NULL);
    std::cerr << "can't execute dc" << std::endl;
}