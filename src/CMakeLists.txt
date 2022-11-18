cmake_minimum_required(VERSION 3.16)
project(PointH LANGUAGES CXX C)

find_package(Arcane REQUIRED)

add_executable(PointH main.cc)

arcane_add_arcane_libraries_to_target(PointH)
target_include_directories(PointH PUBLIC . ${CMAKE_CURRENT_BINARY_DIR})
