#include "thread_functions.h"
/*
*   
    @param LVOID lParam параметр
*/
DWORD WINAPI MinMaxThreadFunc(LPVOID lpParam) {
    ThreadData* data = static_cast<ThreadData*>(lpParam);
    std::vector<int>& array = *(data->array);

    if (array.empty()) {
        std::cout << "Error: Empty array in min_max thread." << std::endl;
        return 1;
    }

    int min_index = 0;
    int max_index = 0;

    for (size_t i = 1; i < array.size(); ++i) {
        // Check for minimum
        if (array[i] < array[min_index]) {
            min_index = i;
        }
        Sleep(7); // Sleep for 7 milliseconds after comparison

        // Check for maximum
        if (array[i] > array[max_index]) {
            max_index = i;
        }
        Sleep(7); // Sleep for 7 milliseconds after comparison
    }

    std::cout << "Min element: " << array[min_index] << " at index " << min_index << std::endl;
    std::cout << "Max element: " << array[max_index] << " at index " << max_index << std::endl;

    data->min_index = min_index;
    data->max_index = max_index;

    return 0;
}

DWORD WINAPI AverageThreadFunc(LPVOID lpParam) {
    ThreadData* data = static_cast<ThreadData*>(lpParam);
    std::vector<int>& array = *(data->array);

    if (array.empty()) {
        std::cout << "Error: Empty array in average thread." << std::endl;
        return 1;
    }

    double sum = 0;

    for (const auto& element : array) {
        sum += element;
        Sleep(12); // Sleep for 12 milliseconds after each addition
    }

    double average = sum / array.size();
    data->average = average;

    std::cout << "Average value: " << average << std::endl;

    return 0;
}