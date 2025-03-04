#include <iostream>
#include <vector>
#include <iomanip>
#include <thread>
#include <chrono>
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

    std::thread min_max_thread(MinMaxThreadFunc, std::ref(thread_data));
    std::thread average_thread(AverageThreadFunc, std::ref(thread_data));

    std::cout << "Waiting for threads to complete..." << std::endl;
    min_max_thread.join();
    average_thread.join();

    std::cout << "Replacing min and max elements with average..." << std::endl;
    array[thread_data.min_index] = thread_data.average;
    array[thread_data.max_index] = thread_data.average;

    printArray(array, "Modified array");

    std::cout << "Program completed successfully." << std::endl;
    return 0;
}