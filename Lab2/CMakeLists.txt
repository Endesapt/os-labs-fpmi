cmake_minimum_required(VERSION 3.10)
project(threads_lab)

set(CMAKE_CXX_STANDARD 17)

# Add the main executable
add_executable(threads_lab 
    src/main.cpp
    src/thread_functions.cpp
)

# Link with the necessary libraries
target_link_libraries(threads_lab)

enable_testing()
add_subdirectory(tests)