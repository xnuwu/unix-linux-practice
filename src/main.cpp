#include <iostream>
#include "tty/playAgain.cpp"

int main(int argc, char* argv[]) {
    int response;
    ttyMode(1);
    set_cr_noecho_mode();
    set_no_delay_mode();
    response = getResponse();
    ttyMode(0);
    std::cout << "response:" << response << std::endl;
}