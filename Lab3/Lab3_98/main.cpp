#include <windows.h>
#include <iostream>
#include <vector>
#include "Thread.h"





int main() {
	
	int arraySize;
	std::cout << "Enter array size: ";
	std::cin >> arraySize;

	int* array = new int[arraySize]();

	int numThreads;
	std::cout << "Enter number of marker threads: ";
	std::cin >> numThreads;

	CRITICAL_SECTION arrayCritSection;
	CRITICAL_SECTION consoleCritSection;
	InitializeCriticalSection(&consoleCritSection);
	InitializeCriticalSection(&arrayCritSection);

	HANDLE startEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	HANDLE* cannotContinueEvents = new HANDLE[numThreads];
	HANDLE* continueOrTerminateEvents = new HANDLE[numThreads];
	HANDLE* terminateEvents = new HANDLE[numThreads];
	HANDLE* threadHandles = new HANDLE[numThreads];

	std::vector<int>* markedElements = new std::vector<int>[numThreads];

	for (int i = 0; i < numThreads; i++) {
		cannotContinueEvents[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
		continueOrTerminateEvents[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
		terminateEvents[i] = CreateEvent(NULL, FALSE, FALSE, NULL);

		ThreadData* data = new ThreadData;
		data->id = i + 1;
		data->array = array;
		data->arraySize = arraySize;
		data->startEvent = startEvent;
		data->cannotContinueEvents = cannotContinueEvents;
		data->continueOrTerminateEvents = continueOrTerminateEvents;
		data->terminateEvents = terminateEvents;
		data->arrayCritSection = &arrayCritSection;
		data->numThreads = numThreads;
		data->markedElements = markedElements;
		data->consoleCritSection = &consoleCritSection;

		threadHandles[i] = CreateThread(NULL, 0, MarkerThread, data, 0, NULL);
	}

	SetEvent(startEvent);

	bool* activeThreadFlags = new bool[numThreads];
	for (int i = 0; i < numThreads; i++) {
		activeThreadFlags[i] = true;
	}
	int activeThreadsCount = numThreads;

	while (activeThreadsCount > 0) {
		HANDLE* activeEvents = new HANDLE[activeThreadsCount];
		int activeIndex = 0;

		for (int i = 0; i < numThreads; i++) {
			if (activeThreadFlags[i]) {
				activeEvents[activeIndex++] = cannotContinueEvents[i];
			}
		}

		WaitForMultipleObjects(activeThreadsCount, activeEvents, TRUE, INFINITE);
		delete[] activeEvents;
		std::cout << "Array: ";
		for (int i = 0; i < arraySize; i++) {
			std::cout << array[i] << " ";
		}
		std::cout << std::endl;
		
		while (true) {
			int threadToTerminate;
			std::cout << "Enter marker thread number to terminate (1-" << numThreads << "): ";
			std::cin >> threadToTerminate;


			if (threadToTerminate < 1 || threadToTerminate > numThreads) {
				std::cout << "Invalid thread number" << std::endl;
			}
			else if (!activeThreadFlags[threadToTerminate - 1]) {
				std::cout << "Thread " << threadToTerminate << " is already terminated" << std::endl;
			}
			else {
				SetEvent(terminateEvents[threadToTerminate - 1]);
				SetEvent(continueOrTerminateEvents[threadToTerminate - 1]);

				WaitForSingleObject(threadHandles[threadToTerminate - 1], INFINITE);
				CloseHandle(threadHandles[threadToTerminate - 1]);

				activeThreadFlags[threadToTerminate - 1] = false;
				activeThreadsCount--;
				break;
			}
		}



		std::cout << "Array after thread termination: ";
		for (int i = 0; i < arraySize; i++) {
			std::cout << array[i] << " ";
		}
		std::cout << std::endl;

		for (int i = 0; i < numThreads; i++) {
			if (activeThreadFlags[i]) {
				SetEvent(continueOrTerminateEvents[i]);
			}
		}
	}

	DeleteCriticalSection(&arrayCritSection);
	CloseHandle(startEvent);

	for (int i = 0; i < numThreads; i++) {
		CloseHandle(cannotContinueEvents[i]);
		CloseHandle(continueOrTerminateEvents[i]);
		CloseHandle(terminateEvents[i]);
	}

	delete[] array;
	delete[] cannotContinueEvents;
	delete[] continueOrTerminateEvents;
	delete[] terminateEvents;
	delete[] threadHandles;
	delete[] markedElements;
	delete[] activeThreadFlags;

	std::cout << "All threads terminated. Program finished." << std::endl;
	return 0;
}