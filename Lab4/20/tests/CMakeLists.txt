add_executable(ipc_tests ipc_tests.cpp)

target_link_libraries(ipc_tests PRIVATE GTest::gtest_main)

# ���� ��� common.h � utils.h �� ���������� src
target_include_directories(ipc_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

include(GoogleTest)
gtest_discover_tests(ipc_tests)