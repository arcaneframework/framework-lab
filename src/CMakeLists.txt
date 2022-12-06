cmake_minimum_required(VERSION 3.16)
project(ParallelMngArcane LANGUAGES CXX C)

find_package(Arcane REQUIRED)

add_executable(ParallelMngArcane main.cc HybridArcane.cc ShMemArcane.cc MpiArcane.cc
  LauncherModule.cc Launcher_axl.h)

arcane_generate_axl(Launcher)

arcane_add_arcane_libraries_to_target(ParallelMngArcane)
target_include_directories(ParallelMngArcane PUBLIC . ${CMAKE_CURRENT_BINARY_DIR})
configure_file(ParallelMngArcane.config ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)