add_executable (Reporter "Reporter.cpp" "Reporter.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Main PROPERTY CXX_STANDARD 20)
endif()