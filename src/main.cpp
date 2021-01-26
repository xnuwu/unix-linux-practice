#define DEBUG_LOG

#include "tty/tty.cpp"

int main(int argc, char* argv[])
{
    readTTYAndSave("ttyData.txt");
    return 0;
}