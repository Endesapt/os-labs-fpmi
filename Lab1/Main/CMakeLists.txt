﻿
add_executable (Main "Main.cpp" "Main.h")
add_dependencies(Main Creator)
add_dependencies(Main Reporter)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Main PROPERTY CXX_STANDARD 20)
endif()

