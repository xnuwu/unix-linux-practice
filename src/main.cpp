#include <iostream>
#include "test/caculatorTest.cpp"
#include "tty/ttyTest.cpp"

#define BUFF_SIZE 512

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}