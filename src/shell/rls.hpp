#ifndef RLS
#define RLS

#define BUFF_SIZE 256
#define HOST_LEN 256
#define PORTNUM 6789
#define oops(msg) {std::perror(msg);std::exit(1);}

void rls(const char* host, const char* path);
void rlsd();
void sonitize(char* path);

#endif