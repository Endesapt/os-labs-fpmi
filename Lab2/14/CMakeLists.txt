# CMakeList.txt: проект CMake для 98; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project(threads_lab_14)

set(CMAKE_CXX_STANDARD 14)

# Add the main executable
add_executable(threads_lab 
    src/main.cpp
    src/thread_functions.cpp
)

# Link with the necessary libraries
target_link_libraries(threads_lab)

enable_testing()
add_subdirectory(tests)