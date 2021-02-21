#ifndef SMSH
#define SMSH

#include <stdio.h>
#define DEL_PROMPT ">"
#define IS_DELIME(C) ((C)==' '||(C)=='\t')
#define BUFFER_SIZE 32

//basic shell implement
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

//controlFlow
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

//builtin command handle and environ variable
int builtInCommand(char* cmd[], int* resultPtr);

struct var {
    char* str;  //eg:label=value
    int global; //0-no global,1-global
};

#define MAX_VARS 1024
static struct var environTab[MAX_VARS];
extern char** environ;

//environ variable
int VStore(const char* name,const char* value);
char* VLookUp(const char* name);
int VExport(const char* name);
void VList();

static struct var* findItem(const char* name, int firstBlank);
char** VTable2environ();
int VEnviron2Table(char* env[]);

int assign(char* cmd);
int okName(char* name);
char* buildEnvStr(const char* name,const char* value);

#endif