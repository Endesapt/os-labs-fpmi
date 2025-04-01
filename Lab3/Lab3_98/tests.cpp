#include <windows.h>
#include <iostream>
#include <vector>
#include <cassert>
#include "Thread.h"

// Include main program functionality
// Forward declarations for functions to be tested
struct ThreadData;
DWORD WINAPI MarkerThread(LPVOID lpParam);

// Simple test for array initialization
void testArrayInitialization() {
    std::cout << "Testing array initialization..." << std::endl;

    int arraySize = 10;
    int* array = new int[arraySize]();

    bool allZeros = true;
    for (int i = 0; i < arraySize; i++) {
        if (array[i] != 0) {
            allZeros = false;
            break;
        }
    }

    if (allZeros) {
        std::cout << "PASS: Array initialized with zeros" << std::endl;
    }
    else {
        std::cout << "FAIL: Array not initialized with zeros" << std::endl;
    }

    delete[] array;
}

// Test critical section functionality
void testCriticalSection() {
    std::cout << "Testing critical section..." << std::endl;

    CRITICAL_SECTION critSection;
    InitializeCriticalSection(&critSection);

    // Test if we can enter and leave the critical section
    EnterCriticalSection(&critSection);
    LeaveCriticalSection(&critSection);
    std::cout << "PASS: Can enter and leave critical section" << std::endl;

    DeleteCriticalSection(&critSection);
}

// Test event functionality
void testEvents() {
    std::cout << "Testing events..." << std::endl;

    HANDLE event = CreateEvent(NULL, TRUE, FALSE, NULL);

    // Test initial state
    DWORD initialState = WaitForSingleObject(event, 0);
    if (initialState == WAIT_TIMEOUT) {
        std::cout << "PASS: Event is initially non-signaled" << std::endl;
    }
    else {
        std::cout << "FAIL: Event should be non-signaled initially" << std::endl;
    }

    // Test setting the event
    SetEvent(event);
    DWORD afterSetState = WaitForSingleObject(event, 0);
    if (afterSetState == WAIT_OBJECT_0) {
        std::cout << "PASS: Event is signaled after SetEvent" << std::endl;
    }
    else {
        std::cout << "FAIL: Event should be signaled after SetEvent" << std::endl;
    }

    // Clean up
    CloseHandle(event);
}

// Run all tests
int main() {
    std::cout << "Running simple tests for Lab3_98..." << std::endl;

    testArrayInitialization();
    testCriticalSection();
    testEvents();

    std::cout << "All tests completed." << std::endl;

    return 0;
}