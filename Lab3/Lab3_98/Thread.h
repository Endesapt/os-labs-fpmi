#include <windows.h>
#include <iostream>
#include <vector>
struct ThreadData {
	int id;
	int* array;
	int arraySize;
	HANDLE startEvent;
	HANDLE* cannotContinueEvents;
	HANDLE* continueOrTerminateEvents;
	HANDLE* terminateEvents;
	CRITICAL_SECTION* arrayCritSection;
	CRITICAL_SECTION* consoleCritSection;
	int numThreads;
	std::vector<int>* markedElements;
};
DWORD WINAPI MarkerThread(LPVOID lpParam) {
	ThreadData* data = static_cast<ThreadData*>(lpParam);

	WaitForSingleObject(data->startEvent, INFINITE);

	srand(data->id);

	int markedCount = 0;
	std::vector<int>& markerElements = data->markedElements[data->id - 1];

	bool terminate = false;
	while (!terminate) {
		int randomIndex = rand() % data->arraySize;

		EnterCriticalSection(data->arrayCritSection);

		if (data->array[randomIndex] == 0) {
			Sleep(5);
			data->array[randomIndex] = data->id;
			markerElements.push_back(randomIndex);
			markedCount++;
			Sleep(5);
			LeaveCriticalSection(data->arrayCritSection);
		}
		else {
			LeaveCriticalSection(data->arrayCritSection);
			EnterCriticalSection(data->consoleCritSection);
			std::cout << "Marker " << data->id << ": marked " << markedCount
				<< " elements, cannot mark element with index " << randomIndex << std::endl;
			LeaveCriticalSection(data->consoleCritSection);

			SetEvent(data->cannotContinueEvents[data->id - 1]);

			DWORD waitResult = WaitForSingleObject(
				data->continueOrTerminateEvents[data->id - 1], INFINITE);

			DWORD terminateSignal = WaitForSingleObject(
				data->terminateEvents[data->id - 1], 0);

			if (terminateSignal == WAIT_OBJECT_0) {
				EnterCriticalSection(data->arrayCritSection);
				for (size_t i = 0; i < markerElements.size(); i++) {
					data->array[markerElements[i]] = 0;
				}
				LeaveCriticalSection(data->arrayCritSection);
				terminate = true;
			}
		}
	}

	return 0;
}
