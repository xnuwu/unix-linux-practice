#include "tty.cpp"
#include "gtest/gtest.h"

TEST(TTY, echoState) {
    GTEST_ASSERT_EQ(true, echoState());
}

TEST(TTY, setEchoState) {
    GTEST_ASSERT_EQ(true, setEchoState(true));
}