# ��������� ����������� ���� ��� ������
add_executable(ipc_tests ipc_tests.cpp)

# ������� ����� � GoogleTest
# gtest_main ������������� ��������� main ������� ��� ������
target_link_libraries(ipc_tests PRIVATE GTest::gtest_main)

# �������� common.h �� ���������� src
# ${CMAKE_SOURCE_DIR} ��������� �� ������ ������� (ipc_project)
target_include_directories(ipc_tests PRIVATE ${CMAKE_SOURCE_DIR}/src/)

# ����������� � ���������� ������ � CTest
include(GoogleTest)
gtest_discover_tests(ipc_tests)