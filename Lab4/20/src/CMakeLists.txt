# ����������� ���� ��� Receiver (�����������������)
add_executable(Receiver Receiver.cpp common.h utils.h)

# ����������� ���� ��� Sender (�����������������)
add_executable(Sender Sender.cpp common.h utils.h)

# ���������, ��� CMake ������ ��������� ����������� �����
set_target_properties(Receiver Sender PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
