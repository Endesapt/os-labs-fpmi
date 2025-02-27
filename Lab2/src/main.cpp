#include <iostream>
#include <vector>
#include <iomanip>
#include <windows.h>
#include "thread_functions.h"

void printArray(const std::vector<int>& array, const std::string& label) {
    std::cout << label << ": ";
    for (const auto& element : array) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}

int main() {
    // 1. Create an array of integers
    int size;
    std::cout << "Enter array size: ";
    std::cin >> size;

    if (size <= 0) {
        std::cout << "Invalid array size. Exiting." << std::endl;
        return 1;
    }

    std::vector<int> array(size);
    std::cout << "Enter array elements:" << std::endl;
    for (int i = 0; i < size; ++i) {
        std::cout << "Element " << i << ": ";
        std::cin >> array[i];
    }

    // Print the original array
    printArray(array, "Original array");

    // Create thread data
    ThreadData thread_data;
    thread_data.array = &array;

    // 2. Create min_max and average threads
    HANDLE hMinMax = CreateThread(
        NULL,                   // Default security attributes
        0,                      // Default stack size
        MinMaxThreadFunc,       // Thread function
        &thread_data,           // Thread function parameter
        0,                      // Default creation flags
        NULL                    // Receive thread identifier
    );

    if (hMinMax == NULL) {
        std::cout << "Error creating min_max thread." << std::endl;
        return 1;
    }

    HANDLE hAverage = CreateThread(
        NULL,                   // Default security attributes
        0,                      // Default stack size
        AverageThreadFunc,      // Thread function
        &thread_data,           // Thread function parameter
        0,                      // Default creation flags
        NULL                    // Receive thread identifier
    );

    if (hAverage == NULL) {
        std::cout << "Error creating average thread." << std::endl;
        CloseHandle(hMinMax);
        return 1;
    }

    // 3. Wait for threads to complete
    std::cout << "Waiting for threads to complete..." << std::endl;
    WaitForSingleObject(hMinMax, INFINITE);
    WaitForSingleObject(hAverage, INFINITE);

    // Close thread handles
    CloseHandle(hMinMax);
    CloseHandle(hAverage);

    // 4. Replace min and max elements with average
    std::cout << "Replacing min and max elements with average..." << std::endl;
    array[thread_data.min_index] = thread_data.average;
    array[thread_data.max_index] = thread_data.average;

    // Print the modified array
    printArray(array, "Modified array");

    std::cout << "Program completed successfully." << std::endl;
    return 0;
}