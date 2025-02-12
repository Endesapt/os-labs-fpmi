#include <gtest/gtest.h>
#include "FibonacciGenerator.h"

TEST(FibonacciGeneratorTest, GenerateZeroThrows) {
   FibonacciGenerator<int> generator;
   EXPECT_THROW(generator.generate(0), std::invalid_argument);
}

TEST(FibonacciGeneratorTest, GenerateOneElement) {
    FibonacciGenerator<int> generator;
    std::vector<int> result = generator.generate(1);
    ASSERT_EQ(1, result.size());
    EXPECT_EQ(0, result[0]);
}

TEST(FibonacciGeneratorTest, GenerateFiveElements) {
    FibonacciGenerator<int> generator;
    std::vector<int> result = generator.generate(5);
    ASSERT_EQ(5, result.size());
    EXPECT_EQ(0, result[0]);
    EXPECT_EQ(1, result[1]);
    EXPECT_EQ(1, result[2]);
    EXPECT_EQ(2, result[3]);
    EXPECT_EQ(3, result[4]);
}

TEST(FibonacciGeneratorTest, OverflowTest) {
    FibonacciGenerator<unsigned char> generator;
    EXPECT_THROW(generator.generate(52), std::overflow_error);
}