#include "more/more.cpp"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) 
{
    if(argc == 1) {
        std::cout << "read from cli" << std::endl;
        displayContent(std::cin);
    }else{
        std::cout << "read from input stream, there are " << argc - 1 <<" file ready" << std::endl;
        int currentFileIndex = 1;
        while(currentFileIndex < argc) {
            std::ifstream ifs(argv[currentFileIndex]);
            displayContent(ifs);
            currentFileIndex++;
        }
    }
}