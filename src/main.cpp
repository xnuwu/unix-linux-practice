#include <iostream>
#include "tty/playAgain.cpp"
#include <signal.h>

int main(int argc, char* argv[]) {

    int response;
    ttyMode(1);
    set_cr_noecho_mode();
    set_no_delay_mode();

    signal(SIGINT, ctrlCHandler);
    signal(SIGQUIT, SIG_IGN);
    response = getResponse();
    ttyMode(0);
    std::cout << "response:" << response << std::endl;
}