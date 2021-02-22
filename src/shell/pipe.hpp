#ifndef PIPE
#define PIPE

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <error.h>

#define BUFF_SIZE 512
#define CHILD_MSG "hello, i'm child!\r\n"
#define PARENT_MSG "hey child, i'm parent!\r\n"
#define OOPS(M, X) {perror(M);exit(X);}

void testPipe();

#endif