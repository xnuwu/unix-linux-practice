#include "cp.cpp"

int main(int argc, char* argv[])
{
    if(argc != 3) {
        std::cout << "usage: " << argv[0] << " src dst" << std::endl;
        exit(1);
    }

    cp(argv[1], argv[2]);
    
    return 0;
}