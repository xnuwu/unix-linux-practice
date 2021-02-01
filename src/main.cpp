#include "pwd/pwd.cpp"

int main(int argc, char* argv[])
{
    printCurrentPath(getINode("."));
    std::cout << std::endl;
    return 0;
}