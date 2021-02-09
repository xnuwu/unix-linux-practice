#ifndef BOUNCE_LD
#define BOUNCE_LD

#include <string>

void bounce(const std::string& msg);
void bounceMoveMessage(int signum);
int bounceSetTicker(int delay);

#endif