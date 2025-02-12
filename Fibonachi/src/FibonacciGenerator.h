#ifndef FIBONACCI_GENERATOR_H
#define FIBONACCI_GENERATOR_H

#include <vector>
#include <stdexcept>
#include <limits>

template<typename T>
class FibonacciGenerator {
public:
    std::vector<T> generate(int n) {
        if (n <= 0) {
            throw std::invalid_argument("n must be positive");
        }

        std::vector<T> result;
        if (n >= 1) {
            result.push_back(0);
        }
        if (n >= 2) {
            T next = 1;
            result.push_back(next);
        }

        for (int i = 2; i < n; ++i) {
            T a = result[i - 1];
            T b = result[i - 2];
            if (willOverflow(a, b)) {
                throw std::overflow_error("Fibonacci sequence overflow");
            }
            T next = a + b;
            result.push_back(next);
        }

        return result;
    }

private:
    bool willOverflow(T a, T b) const {
        if (a > 0 && b > 0) {
            return a > (std::numeric_limits<T>::max() - b);
        }
        return false;
    }
};

#endif // FIBONACCI_GENERATOR_H