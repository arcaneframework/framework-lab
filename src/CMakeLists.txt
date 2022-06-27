set(PROJECT_NAME csv)
cmake_minimum_required(VERSION 3.16)
project(${PROJECT_NAME} LANGUAGES CXX)

find_package(Arcane REQUIRED)

add_executable(${PROJECT_NAME} 
main.cc )

add_library(CsvOutput csv_output/CsvOutputService.cc)
target_include_directories(CsvOutput PUBLIC .)
target_link_libraries(CsvOutput PUBLIC arcane_core)

add_library(SimpleTableOutputTester tester/SimpleTableOutputTesterService.cc)
target_include_directories(SimpleTableOutputTester PUBLIC .)
target_link_libraries(SimpleTableOutputTester PUBLIC arcane_core)

add_library(CsvExample1 examples/CsvExample1Module.cc)
target_include_directories(CsvExample1 PUBLIC .)
target_link_libraries(CsvExample1 PUBLIC arcane_core)

target_link_libraries(${PROJECT_NAME} PRIVATE CsvOutput SimpleTableOutputTester CsvExample1)

arcane_target_add_axl(CsvOutput csv_output/CsvOutput)
arcane_target_add_axl(SimpleTableOutputTester tester/SimpleTableOutputTester)
arcane_target_add_axl(CsvExample1 examples/CsvExample1)

arcane_add_arcane_libraries_to_target(${PROJECT_NAME})
target_compile_options(${PROJECT_NAME} PUBLIC -Wpedantic)
target_include_directories(${PROJECT_NAME} PUBLIC . ${CMAKE_CURRENT_BINARY_DIR})
configure_file(${PROJECT_NAME}.config ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)