cmake_minimum_required(VERSION 3.16)
project(mpA LANGUAGES CXX C)

find_package(Arcane REQUIRED)

add_executable(mpA main.cc ShMemArcane.cc MpiArcane.cc
  SayHelloModule.cc SayHello_axl.h)

arcane_generate_axl(SayHello)

arcane_add_arcane_libraries_to_target(mpA)
target_include_directories(mpA PUBLIC . ${CMAKE_CURRENT_BINARY_DIR})
configure_file(HelloWorld.config ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)