#pragma once

#include <windows.h>
#include <vector>
#include <iostream>

/**
 * Structure to pass data between threads.
 */
struct ThreadData {
    std::vector<int>* array;    // Pointer to the array
    int min_index;              // Index of minimum element
    int max_index;              // Index of maximum element
    double average;             // Average value of elements
};

/**
 * Thread function for finding minimum and maximum elements.
 *
 * @param lpParam Pointer to ThreadData structure
 * @return Thread exit code
 */
DWORD WINAPI MinMaxThreadFunc(LPVOID lpParam);

/**
 * Thread function for calculating average value.
 *
 * @param lpParam Pointer to ThreadData structure
 * @return Thread exit code
 */
DWORD WINAPI AverageThreadFunc(LPVOID lpParam);