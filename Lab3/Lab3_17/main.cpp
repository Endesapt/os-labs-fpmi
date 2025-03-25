#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <random>
#include <atomic>

int main() {
    // Запросить размерность массива
    int arraySize;
    std::cout << "Enter array size: ";
    std::cin >> arraySize;

    // Создать и инициализировать вектор
    std::vector<int> array(arraySize, 0);

    // Запросить количество потоков marker
    int numThreads;
    std::cout << "Enter number of marker threads: ";
    std::cin >> numThreads;

    // Мьютекс для защиты доступа к массиву
    std::mutex arrayMutex;

    // Переменные для старта потоков
    std::condition_variable startCv;
    bool startFlag = false;

    // Переменные для синхронизации, когда поток не может продолжать работу
    std::mutex barrierMutex;
    std::condition_variable barrierCv;
    std::atomic<int> threadsAtBarrier(0);

    // Переменные для сигналов продолжения/завершения
    std::vector<bool> continueFlags(numThreads, false);
    std::vector<bool> terminateFlags(numThreads, false);
    std::mutex continueMutex;
    std::condition_variable continueCv;

    // Вектор для хранения помеченных индексов для каждого потока
    std::vector<std::vector<int>> markedElements(numThreads);

    // Вектор потоков
    std::vector<std::thread> threads;

    // Функция для потоков marker
    auto markerFunction = [&](int id) {
        // Ждать сигнал на начало работы
        {
            std::unique_lock<std::mutex> lock(arrayMutex);
            startCv.wait(lock, [&startFlag]() { return startFlag; });
        }

        // Инициализировать генератор случайных чисел
        std::mt19937 gen(id);
        std::uniform_int_distribution<> distrib(0, arraySize - 1);

        int markedCount = 0;
        std::vector<int>& threadMarkedElements = markedElements[id - 1];

        while (true) {
            // Генерировать случайное число
            int randomIndex = distrib(gen);

            bool canContinue = false;
            {
                std::lock_guard<std::mutex> lock(arrayMutex);

                if (array[randomIndex] == 0) {
                    // Нашли незанятую ячейку
                    std::this_thread::sleep_for(std::chrono::milliseconds(5));
                    array[randomIndex] = id;
                    threadMarkedElements.push_back(randomIndex);
                    markedCount++;
                    std::this_thread::sleep_for(std::chrono::milliseconds(5));
                    canContinue = true;
                }
            }

            if (!canContinue) {
                // Ячейка занята другим потоком
                {
                    std::lock_guard<std::mutex> lock(arrayMutex);
                    std::cout << "Marker " << id << ": marked " << markedCount
                        << " elements, cannot mark element with index " << randomIndex << std::endl;
                }

                // Сигнал о невозможности продолжения работы
                {
                    std::lock_guard<std::mutex> lock(barrierMutex);
                    threadsAtBarrier++;
                }
                barrierCv.notify_all();

                // Ждать сигнал на продолжение или завершение
                {
                    std::unique_lock<std::mutex> lock(continueMutex);
                    continueCv.wait(lock, [&continueFlags, id]() { return continueFlags[id - 1]; });
                    continueFlags[id - 1] = false;
                }

                // Проверить, нужно ли завершить работу
                if (terminateFlags[id - 1]) {
                    // Получен сигнал на завершение работы
                    std::lock_guard<std::mutex> lock(arrayMutex);
                    // Заполнить нулями помеченные элементы
                    for (int index : threadMarkedElements) {
                        array[index] = 0;
                    }
                    break;
                }
            }
        }
        };

    // Создать и запустить потоки
    for (int i = 0; i < numThreads; i++) {
        threads.emplace_back(markerFunction, i + 1);
    }

    // Дать сигнал на начало работы всех потоков
    {
        std::lock_guard<std::mutex> lock(arrayMutex);
        startFlag = true;
    }
    startCv.notify_all();

    // Массив для отслеживания активных потоков
    std::vector<bool> activeThreads(numThreads, true);
    int activeThreadCount = numThreads;

    while (activeThreadCount > 0) {
        // Ждать, пока все активные потоки не подадут сигналы о невозможности продолжения
        {
            std::unique_lock<std::mutex> lock(barrierMutex);
            barrierCv.wait(lock, [&threadsAtBarrier, &activeThreadCount]() {
                return threadsAtBarrier.load() == activeThreadCount;
                });
        }

        // Вывести содержимое массива
        std::cout << "Array: ";
        for (int value : array) {
            std::cout << value << " ";
        }
        std::cout << std::endl;

        // Запросить номер потока для завершения
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
            // Подать сигнал на завершение выбранному потоку
            {
                std::lock_guard<std::mutex> lock(continueMutex);
                terminateFlags[threadToTerminate - 1] = true;
                continueFlags[threadToTerminate - 1] = true;
            }
            continueCv.notify_all();

            // Ждать завершения выбранного потока
            threads[threadToTerminate - 1].join();

            // Отметить поток как неактивный
            activeThreads[threadToTerminate - 1] = false;
            activeThreadCount--;
            terminatedThread = true;
        }

        // Вывести содержимое массива только если завершили поток
        if (terminatedThread) {
            std::cout << "Array after thread termination: ";
            for (int value : array) {
                std::cout << value << " ";
            }
            std::cout << std::endl;
        }

        // Сбросить счетчик потоков на барьере и подать сигнал на продолжение работы оставшимся потокам
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

    // Убедиться, что все потоки завершены
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    std::cout << "All threads terminated. Program finished." << std::endl;
    return 0;
}