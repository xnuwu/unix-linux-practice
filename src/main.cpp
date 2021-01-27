#include "ls/ls.cpp"

int main(int argc, char* argv[])
{
    int opt;
    int paramIndex;
    listDirectoryOptions options;

    while((opt = getopt(argc, argv, "als")) != -1) {
        switch (opt)
        {
            case 'a':
                options.displayAll = true;
                break;

            case 'l':
                options.displayDetail = true;
                break;

            case '?':
                std::cout << "Usage ls -a -l /dirname" << std::endl;
                break;

            default:
                break;
        }
    }

    if(optind == argc) {
        listDirectory(".", options);
    }else if(optind < argc) {
        const char* path = argv[optind];
        while(optind < argc) {
            std::cout << argv[optind] << ":" << std::endl;
            listDirectory(argv[optind], options);
            std::cout << std::endl;
            optind++;
        }
    }
    
    return 0;
}