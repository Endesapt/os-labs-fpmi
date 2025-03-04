# Find Google Test package
find_package(GTest QUIET)

# If not found, fetch it
if(NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
    )
    FetchContent_MakeAvailable(googletest)
endif()
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add the test executable
add_executable(thread_tests 
    thread_tests.cpp
    ${CMAKE_SOURCE_DIR}/src/thread_functions.cpp
)

# Include directories
target_include_directories(thread_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Link with Google Test
target_link_libraries(thread_tests PRIVATE gtest_main)


# Add the test
add_test(NAME thread_tests COMMAND thread_tests)