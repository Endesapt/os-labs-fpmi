# ����������� ���� ��� Receiver
add_executable(Receiver Receiver.cpp common.h)

# ����������� ���� ��� Sender
add_executable(Sender Sender.cpp common.h)

# ���������, ��� CMake ������ ��������� ����������� ����� (��������, � ����� ����-����������)
set_target_properties(Receiver Sender PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
