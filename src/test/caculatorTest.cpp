#include <limits.h>
#include "caculator.cpp"
#include "gtest/gtest.h"

TEST(CaculatorTest, Add) {
    GTEST_ASSERT_EQ(2, add(1, 1));
    // GTEST_ASSERT_EQ(3, add(1, 1)) << "this test will failed";
}