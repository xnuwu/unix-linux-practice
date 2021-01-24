#define DEBUG_LOG

#include "logoutTTY/logoutTTY.cpp"
#include <cstdio>
#include <cstdlib>

int main(int argc, char* argv[])
{
    logoutTTY("reboot");
    return 0;
}