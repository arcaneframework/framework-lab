cmake_minimum_required(VERSION 3.21)
project(ArcanePython LANGUAGES C CXX)

find_package(Arcane REQUIRED)
find_program(DOTNET_EXEC dotnet)
if (NOT DOTNET_EXEC)
  message(FATAL_ERROR "Can not find 'dotnet' executable in path")
endif()

add_custom_command(OUTPUT Test.dll
  COMMAND ${DOTNET_EXEC} publish -o ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/csharp/Test.csproj
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/csharp/Test.csproj ${CMAKE_CURRENT_SOURCE_DIR}/csharp/CSharpCallerModule.cs
)
add_custom_target(build_dotnet
  DEPENDS Test.dll
)
add_executable(ArcanePython SimpleCallModule.cc main.cc SimpleCall_axl.h)
add_dependencies(ArcanePython build_dotnet)

arcane_generate_axl(SimpleCall)
arcane_add_arcane_libraries_to_target(ArcanePython)
target_include_directories(ArcanePython PUBLIC . ${CMAKE_CURRENT_BINARY_DIR})
configure_file(ArcanePython.config ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_test(NAME test_module1
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/ArcanePython -A,DotNetMainAssemblyName=${CMAKE_CURRENT_BINARY_DIR}/Test.dll ${CMAKE_CURRENT_SOURCE_DIR}/ArcanePython.arc
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
