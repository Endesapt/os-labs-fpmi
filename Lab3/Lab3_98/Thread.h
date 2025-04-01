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
    int numThreads;
    std::vector<int>* markedElements;
};
DWORD WINAPI MarkerThread(LPVOID lpParam) {
    ThreadData* data = static_cast<ThreadData*>(lpParam);

    // Ждать сигнал на начало работы
    WaitForSingleObject(data->startEvent, INFINITE);

    // Инициализировать генератор случайных чисел
    srand(data->id);

    int markedCount = 0;
    std::vector<int>& markerElements = data->markedElements[data->id - 1];

    bool terminate = false;
    while (!terminate) {
        // Генерировать случайное число
        int randomIndex = rand() % data->arraySize;

        // Защищаем доступ к массиву
        EnterCriticalSection(data->arrayCritSection);

        if (data->array[randomIndex] == 0) {
            // Нашли незанятую ячейку
            Sleep(5);
            data->array[randomIndex] = data->id;
            markerElements.push_back(randomIndex);
            markedCount++;
            Sleep(5);
            LeaveCriticalSection(data->arrayCritSection);
        }
        else {
            // Ячейка занята другим потоком
            LeaveCriticalSection(data->arrayCritSection);

            std::cout << "Marker " << data->id << ": marked " << markedCount
                << " elements, cannot mark element with index " << randomIndex << std::endl;

            // Сигнал о невозможности продолжения работы
            SetEvent(data->cannotContinueEvents[data->id - 1]);

            // Ждать сигнал на продолжение или завершение
            DWORD waitResult = WaitForSingleObject(
                data->continueOrTerminateEvents[data->id - 1], INFINITE);

            // Проверить, нужно ли завершить работу
            DWORD terminateSignal = WaitForSingleObject(
                data->terminateEvents[data->id - 1], 0);

            if (terminateSignal == WAIT_OBJECT_0) {
                // Получен сигнал на завершение работы
                EnterCriticalSection(data->arrayCritSection);
                // Заполнить нулями помеченные элементы
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
