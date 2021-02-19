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
void fatal(const char* label, const char* detail, int rv);

enum state { NEUTRAL, WANT_THEN, THEN_BLOCK, ELSE_BLOCK };
enum results { SUCCESS, FAILED };

static state if_state = NEUTRAL;
static results if_results = SUCCESS;
static int last_stat = 0;

int process(char* cmd[]);
int sysErr(const char* msg);
int okToExecute();
int isControlFlow(char* line);
int doControlFlow(char* cmd[]);

#endif