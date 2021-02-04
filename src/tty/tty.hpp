#ifndef LOGOUT_TTY
#define LOGOUT_TTY

#include "utmp.h"
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#define TTY_READ_BUFFER_SIZE 128

int logoutTTY(const char* line);
void readTTYAndSaveToFile(const char* savePath);
bool echoState();
bool setEchoState(bool state);

#endif