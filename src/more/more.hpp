#ifndef MORE

#define PAGELEN 16
#define LINELEN 1024
#define ESC \x1b
#define CSI ESC[

#include <iostream>

void displayContent(std::istream& is);
int displayMore();

#endif