#include "thread_functions.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>

void MinMaxThreadFunc(ThreadData& data) {
    std::vector<int>& array = *(data.array);

    if (array.empty()) {
        std::cout << "Error: Empty array in min_max thread." << std::endl;
        return;
    }

    int min_index = 0;
    int max_index = 0;

    for (size_t i = 1; i < array.size(); ++i) {
        // Check for minimum
        if (array[i] < array[min_index]) {
            min_index = i;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(7)); // Sleep for 7 milliseconds after comparison

        // Check for maximum
        if (array[i] > array[max_index]) {
            max_index = i;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(7)); // Sleep for 7 milliseconds after comparison
    }

    std::cout << "Min element: " << array[min_index] << " at index " << min_index << std::endl;
    std::cout << "Max element: " << array[max_index] << " at index " << max_index << std::endl;

    data.min_index = min_index;
    data.max_index = max_index;
}

void AverageThreadFunc(ThreadData& data) {
    std::vector<int>& array = *(data.array);

    if (array.empty()) {
        std::cout << "Error: Empty array in average thread." << std::endl;
        return;
    }

    double sum = 0;

    for (const auto& element : array) {
        sum += element;
        std::this_thread::sleep_for(std::chrono::milliseconds(12)); // Sleep for 12 milliseconds after each addition
    }

    double average = sum / array.size();
    data.average = average;

    std::cout << "Average value: " << average << std::endl;
}