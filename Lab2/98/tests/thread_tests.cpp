#include "thread_functions.h"
#include <gtest/gtest.h>

// Test for MinMaxThreadFunc
TEST(ThreadFunctionsTest, MinMaxThreadFuncTest) {
    std::vector<int> array = { 5, 2, 8, 1, 9, 4 };
    ThreadData data;
    data.array = &array;

    MinMaxThreadFunc(&data);

    EXPECT_EQ(data.min_index, 3);  // Index of minimum element (1)
    EXPECT_EQ(data.max_index, 4);  // Index of maximum element (9)
}

// Test for AverageThreadFunc
TEST(ThreadFunctionsTest, AverageThreadFuncTest) {
    std::vector<int> array = { 5, 2, 8, 1, 9, 4 };
    ThreadData data;
    data.array = &array;

    AverageThreadFunc(&data);

    EXPECT_NEAR(data.average, 4.833333, 0.000001);  // Average of array elements
}

// Test with edge case: single element array
TEST(ThreadFunctionsTest, SingleElementArrayTest) {
    std::vector<int> array = { 42 };
    ThreadData data;
    data.array = &array;

    MinMaxThreadFunc(&data);
    EXPECT_EQ(data.min_index, 0);
    EXPECT_EQ(data.max_index, 0);

    AverageThreadFunc(&data);
    EXPECT_DOUBLE_EQ(data.average, 42.0);
}

// Test thread data structure initialization
TEST(ThreadFunctionsTest, ThreadDataInitializationTest) {
    std::vector<int> array = { 1, 2, 3 };
    ThreadData data;
    data.array = &array;

    // Make sure we can access the array properly
    EXPECT_EQ(data.array->size(), 3);
    EXPECT_EQ((*data.array)[0], 1);
    EXPECT_EQ((*data.array)[1], 2);
    EXPECT_EQ((*data.array)[2], 3);
}