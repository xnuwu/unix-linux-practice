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
void showTTYInfo();
void showBaud(speed_t baudSpeed);
void showTTYFlagInfo(struct termios* ttyInfoPtr);
void showFlagSet(int flagValue, struct flagInfo theBitName[]);
void printScreenDimensions();

#endif