# CMakeList.txt: проект CMake для Lab3_98; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Lab3_98")

# Добавьте источник в исполняемый файл этого проекта.
add_executable (Lab3_98 "main.cpp" "Thread.h")


add_executable (Lab3_98_Test "tests.cpp" "Thread.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Lab3_98 PROPERTY CXX_STANDARD 98)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
