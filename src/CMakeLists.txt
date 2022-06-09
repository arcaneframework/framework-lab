cmake_minimum_required(VERSION 3.16)
project(csv LANGUAGES CXX)

find_package(Arcane REQUIRED)

add_executable(csv 
main.cc 
CsvExample1Module.cc CsvExample1_axl.h 
SimpleTableOutputTesterService.cc SimpleTableOutputTester_axl.h 
CsvOutputService.cc CsvOutputTestService.cc CsvOutput_axl.h)

arcane_generate_axl(CsvExample1)
arcane_generate_axl(SimpleTableOutputTester)
arcane_generate_axl(CsvOutput)
arcane_add_arcane_libraries_to_target(csv)
target_compile_options(csv PUBLIC -Wpedantic)
target_include_directories(csv PUBLIC . ${CMAKE_CURRENT_BINARY_DIR})
configure_file(csv.config ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)