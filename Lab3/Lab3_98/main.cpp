#include <windows.h>
#include <iostream>
#include <vector>

// Структура для передачи данных потоку marker
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

// Функция потока marker
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

int main() {
    // Запросить размерность массива
    int arraySize;
    std::cout << "Enter array size: ";
    std::cin >> arraySize;

    // Создать и инициализировать массив
    int* array = new int[arraySize]();

    // Запросить количество потоков marker
    int numThreads;
    std::cout << "Enter number of marker threads: ";
    std::cin >> numThreads;

    // Инициализировать критическую секцию
    CRITICAL_SECTION arrayCritSection;
    InitializeCriticalSection(&arrayCritSection);

    // Создать массивы дескрипторов событий
    HANDLE startEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    HANDLE* cannotContinueEvents = new HANDLE[numThreads];
    HANDLE* continueOrTerminateEvents = new HANDLE[numThreads];
    HANDLE* terminateEvents = new HANDLE[numThreads];
    HANDLE* threadHandles = new HANDLE[numThreads];

    // Создать векторы для хранения помеченных элементов для каждого потока
    std::vector<int>* markedElements = new std::vector<int>[numThreads];

    // Инициализировать события и создать потоки
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

        threadHandles[i] = CreateThread(NULL, 0, MarkerThread, data, 0, NULL);
    }

    // Дать сигнал на начало работы всех потоков
    SetEvent(startEvent);

    // Массив для отслеживания активных потоков
    bool* activeThreadFlags = new bool[numThreads];
    for (int i = 0; i < numThreads; i++) {
        activeThreadFlags[i] = true;
    }
    int activeThreadsCount = numThreads;

    while (activeThreadsCount > 0) {
        // Подготовить массив событий для ожидания от активных потоков
        HANDLE* activeEvents = new HANDLE[activeThreadsCount];
        int activeIndex = 0;

        // Заполнить массив активных событий
        for (int i = 0; i < numThreads; i++) {
            if (activeThreadFlags[i]) {
                activeEvents[activeIndex++] = cannotContinueEvents[i];
            }
        }

        // Ждать сигналы от всех активных потоков
        WaitForMultipleObjects(activeThreadsCount, activeEvents, TRUE, INFINITE);
        delete[] activeEvents;

        // Вывести содержимое массива
        std::cout << "Array: ";
        for (int i = 0; i < arraySize; i++) {
            std::cout << array[i] << " ";
        }
        std::cout << std::endl;

        // Запросить номер потока для завершения
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
            // Подать сигнал на завершение выбранному потоку
            SetEvent(terminateEvents[threadToTerminate - 1]);
            SetEvent(continueOrTerminateEvents[threadToTerminate - 1]);

            // Ждать завершения выбранного потока
            WaitForSingleObject(threadHandles[threadToTerminate - 1], INFINITE);
            CloseHandle(threadHandles[threadToTerminate - 1]);

            // Отметить поток как неактивный
            activeThreadFlags[threadToTerminate - 1] = false;
            activeThreadsCount--;
        }



        // Вывести содержимое массива
        std::cout << "Array after thread termination: ";
        for (int i = 0; i < arraySize; i++) {
            std::cout << array[i] << " ";
        }
        std::cout << std::endl;

        // Подать сигнал на продолжение работы оставшимся потокам
        for (int i = 0; i < numThreads; i++) {
            if (activeThreadFlags[i]) {
                SetEvent(continueOrTerminateEvents[i]);
            }
        }
    }

    // Освободить ресурсы
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