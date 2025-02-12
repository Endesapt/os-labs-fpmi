#include "FibonacciGenerator.h"
#include <iostream>

int main() {
    try {
        FibonacciGenerator<unsigned int> generator;
        std::vector<unsigned int> fib = generator.generate(10);
        for (size_t i = 0; i < fib.size(); ++i) {
            std::cout << fib[i] << " ";
        }
        std::cout << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}