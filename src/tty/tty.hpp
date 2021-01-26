#ifndef LOGOUT_TTY
#define LOGOUT_TTY

#include "utmp.h"
#include <fcntl.h>

#define TTY_READ_BUFFER_SIZE 128

int logoutTTY(const char* line);
void readTTYAndSaveToFile(const char* savePath);

#endif