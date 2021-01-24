#ifndef LOGOUT_TTY
#define LOGOUT_TTY

#include "utmp.h"
#include <fcntl.h>

int logoutTTY(const char* line);

#endif