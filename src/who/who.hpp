#ifndef WHO
#define WHO

#include <utmp.h>

#define SHOWHOST

void showInfo(const utmp& utmpR);
void showTime(long timeval);

#endif