#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <random>
#include <atomic>
#include <cassert>

// Test vector initialization
void testVectorInitialization() {
    std::cout << "Testing vector initialization..." << std::endl;

    int size = 10;
    std::vector<int> vec(size, 0);

    bool allZeros = true;
    for (int i = 0; i < size; i++) {
        if (vec[i] != 0) {
            allZeros = false;
            break;
        }
    }

    if (allZeros) {
        std::cout << "PASS: Vector initialized with zeros" << std::endl;
    }
    else {
        std::cout << "FAIL: Vector not initialized with zeros" << std::endl;
    }
}

// Test mutex locking functionality
void testMutex() {
    std::cout << "Testing mutex functionality..." << std::endl;

    std::mutex mtx;
    bool lockAcquired = false;

    // Try to lock the mutex
    mtx.lock();
    lockAcquired = true;
    mtx.unlock();

    if (lockAcquired) {
        std::cout << "PASS: Mutex lock and unlock works correctly" << std::endl;
    }
    else {
        std::cout << "FAIL: Could not lock/unlock mutex" << std::endl;
    }
}

// Test condition variable functionality
void testConditionVariable() {
    std::cout << "Testing condition variable..." << std::endl;

    std::mutex mtx;
    std::condition_variable cv;
    bool flag = false;
    bool notificationReceived = false;

    // Create a thread that waits on the condition variable
    std::thread waiter([&]() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&flag]() { return flag; });
        notificationReceived = true;
        });

    // Give the waiter thread time to start waiting
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Set the flag and notify
    {
        std::lock_guard<std::mutex> lock(mtx);
        flag = true;
    }
    cv.notify_one();

    // Wait for the thread to complete
    waiter.join();

    if (notificationReceived) {
        std::cout << "PASS: Condition variable notification works correctly" << std::endl;
    }
    else {
        std::cout << "FAIL: Condition variable notification failed" << std::endl;
    }
}

// Test random number generation
void testRandomGenerator() {
    std::cout << "Testing random number generator..." << std::endl;

    int min = 0;
    int max = 99;
    int seed = 42;

    std::mt19937 gen(seed);
    std::uniform_int_distribution<> distrib(min, max);

    int randomNum = distrib(gen);

    if (randomNum >= min && randomNum <= max) {
        std::cout << "PASS: Random number generator produced value in range ["
            << min << ", " << max << "]: " << randomNum << std::endl;
    }
    else {
        std::cout << "FAIL: Random number out of expected range" << std::endl;
    }
}

int main() {
    std::cout << "Running simple tests for modern C++ marker program..." << std::endl;

    testVectorInitialization();
    testMutex();
    testConditionVariable();
    testRandomGenerator();

    std::cout << "All tests completed." << std::endl;

    return 0;
}