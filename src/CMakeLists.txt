set(PROJECT_NAME csv)
cmake_minimum_required(VERSION 3.16)
project(${PROJECT_NAME} LANGUAGES CXX)

find_package(Arcane REQUIRED)

add_executable(${PROJECT_NAME} 
main.cc )




add_library(CsvOutput simple_table_output/CsvOutputService.cc)
target_include_directories(CsvOutput PUBLIC .)
target_link_libraries(CsvOutput PUBLIC arcane_core)




add_library(SimpleTableOutputTester tester/SimpleTableOutputTesterService.cc)
target_include_directories(SimpleTableOutputTester PUBLIC .)
target_link_libraries(SimpleTableOutputTester PUBLIC arcane_core)




add_library(TableExample1 examples/TableExample1Module.cc)
target_include_directories(TableExample1 PUBLIC .)
target_link_libraries(TableExample1 PUBLIC arcane_core)

add_library(TableExample2 examples/TableExample2Module.cc)
target_include_directories(TableExample2 PUBLIC .)
target_link_libraries(TableExample2 PUBLIC arcane_core)

add_library(TableExample3 examples/TableExample3Module.cc)
target_include_directories(TableExample3 PUBLIC .)
target_link_libraries(TableExample3 PUBLIC arcane_core)


target_link_libraries(${PROJECT_NAME} PRIVATE CsvOutput SimpleTableOutputTester TableExample1 TableExample2 TableExample3)

arcane_target_add_axl(CsvOutput simple_table_output/CsvOutput)
arcane_target_add_axl(SimpleTableOutputTester tester/SimpleTableOutputTester)
arcane_target_add_axl(TableExample1 examples/TableExample1)
arcane_target_add_axl(TableExample2 examples/TableExample2)
arcane_target_add_axl(TableExample3 examples/TableExample3)

arcane_add_arcane_libraries_to_target(${PROJECT_NAME})
target_compile_options(${PROJECT_NAME} PUBLIC -Wpedantic)
target_include_directories(${PROJECT_NAME} PUBLIC . ${CMAKE_CURRENT_BINARY_DIR})
configure_file(${PROJECT_NAME}.config ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)