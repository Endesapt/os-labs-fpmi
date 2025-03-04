#pragma once

#include <vector>

/**
 * Structure to pass data between threads.
 */
struct ThreadData {
    std::vector<int>* array;    // Pointer to the array
    int min_index = 0;          // Index of minimum element
    int max_index = 0;          // Index of maximum element
    double average = 0.0;       // Average value of elements
};

/**
 * Thread function for finding minimum and maximum elements.
 *
 * @param data Reference to ThreadData structure
 */
void MinMaxThreadFunc(ThreadData& data);

/**
 * Thread function for calculating average value.
 *
 * @param data Reference to ThreadData structure
 */
void AverageThreadFunc(ThreadData& data);