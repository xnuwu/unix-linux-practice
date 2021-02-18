#ifndef SMSH
#define SMSH

#include <stdio.h>

#define DEL_PROMPT ">"
#define IS_DELIME(C) ((C)==' '||(C)=='\t')
#define BUFFER_SIZE 32

void runShell();
void setUp();
char* readCommand(const char* prompt, FILE* fp);
char** splitLine(char* line);
char* newstr(char* cp, size_t len);
char* freeList(char** list);
void* emalloc(size_t n);
void* erealloc(void* p, size_t n);
int execute(char* cmd[]);
void fatal(const char* label,const char* detail, int rv);

#endif