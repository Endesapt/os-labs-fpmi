#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <random>
#include <atomic>

int main() {
	int arraySize;
	std::cout << "Enter array size: ";
	std::cin >> arraySize;

	std::vector<int> array(arraySize, 0);

	int numThreads;
	std::cout << "Enter number of marker threads: ";
	std::cin >> numThreads;

	std::mutex arrayMutex;

	std::condition_variable startCv;
	bool startFlag = false;

	std::mutex barrierMutex;
	std::condition_variable barrierCv;
	std::atomic<int> threadsAtBarrier(0);

	std::vector<bool> continueFlags(numThreads, false);
	std::vector<bool> terminateFlags(numThreads, false);
	std::mutex continueMutex;
	std::condition_variable continueCv;

	std::vector<std::vector<int>> markedElements(numThreads);

	std::vector<std::thread> threads;

	auto markerFunction = [&](int id) {
		{
			std::unique_lock<std::mutex> lock(arrayMutex);
			startCv.wait(lock, [&startFlag]() { return startFlag; });
		}

		std::mt19937 gen(id);
		std::uniform_int_distribution<> distrib(0, arraySize - 1);

		int markedCount = 0;
		std::vector<int>& threadMarkedElements = markedElements[id - 1];

		while (true) {
			int randomIndex = distrib(gen);

			bool canContinue = false;
			{
				std::lock_guard<std::mutex> lock(arrayMutex);

				if (array[randomIndex] == 0) {
					std::this_thread::sleep_for(std::chrono::milliseconds(5));
					array[randomIndex] = id;
					threadMarkedElements.push_back(randomIndex);
					markedCount++;
					std::this_thread::sleep_for(std::chrono::milliseconds(5));
					canContinue = true;
				}
			}

			if (!canContinue) {
				{
					std::lock_guard<std::mutex> lock(arrayMutex);
					std::cout << "Marker " << id << ": marked " << markedCount
						<< " elements, cannot mark element with index " << randomIndex << std::endl;
				}

				{
					std::lock_guard<std::mutex> lock(barrierMutex);
					threadsAtBarrier++;
				}
				barrierCv.notify_all();

				{
					std::unique_lock<std::mutex> lock(continueMutex);
					continueCv.wait(lock, [&continueFlags, id]() { return continueFlags[id - 1]; });
					continueFlags[id - 1] = false;
				}

				if (terminateFlags[id - 1]) {
					std::lock_guard<std::mutex> lock(arrayMutex);
					for (int index : threadMarkedElements) {
						array[index] = 0;
					}
					break;
				}
			}
		}
		};

	for (int i = 0; i < numThreads; i++) {
		threads.emplace_back(markerFunction, i + 1);
	}

	{
		std::lock_guard<std::mutex> lock(arrayMutex);
		startFlag = true;
	}
	startCv.notify_all();

	std::vector<bool> activeThreads(numThreads, true);
	int activeThreadCount = numThreads;

	while (activeThreadCount > 0) {
		{
			std::unique_lock<std::mutex> lock(barrierMutex);
			barrierCv.wait(lock, [&threadsAtBarrier, &activeThreadCount]() {
				return threadsAtBarrier.load() == activeThreadCount;
				});
		}

		std::cout << "Array: ";
		for (int value : array) {
			std::cout << value << " ";
		}
		std::cout << std::endl;

		while (true) {
			int threadToTerminate;
			std::cout << "Enter marker thread number to terminate (1-" << numThreads << "): ";
			std::cin >> threadToTerminate;

			bool terminatedThread = false;

			if (threadToTerminate < 1 || threadToTerminate > numThreads) {
				std::cout << "Invalid thread number" << std::endl;
			}
			else if (!activeThreads[threadToTerminate - 1]) {
				std::cout << "Thread " << threadToTerminate << " is already terminated" << std::endl;
			}
			else {
				{
					std::lock_guard<std::mutex> lock(continueMutex);
					terminateFlags[threadToTerminate - 1] = true;
					continueFlags[threadToTerminate - 1] = true;
				}
				continueCv.notify_all();

				threads[threadToTerminate - 1].join();

				activeThreads[threadToTerminate - 1] = false;
				activeThreadCount--;
				terminatedThread = true;
			}


			if (terminatedThread) {
				std::cout << "Array after thread termination: ";
				for (int value : array) {
					std::cout << value << " ";
				}
				std::cout << std::endl;
				break;
			}
		}

		threadsAtBarrier = 0;
		{
			std::lock_guard<std::mutex> lock(continueMutex);
			for (int i = 0; i < numThreads; i++) {
				if (activeThreads[i]) {
					continueFlags[i] = true;
				}
			}
		}
		continueCv.notify_all();
	}

	for (auto& t : threads) {
		if (t.joinable()) {
			t.join();
		}
	}

	std::cout << "All threads terminated. Program finished." << std::endl;
	return 0;
}