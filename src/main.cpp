#include <iostream>
#include <gtest/gtest.h>

#define BUFF_SIZE 512

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}