cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project(ArcaneBenchs VERSION 1.0 LANGUAGES CXX)

# Il ne faut pas compiler la ou se trouve les sources
if(CMAKE_BINARY_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  message(FATAL_ERROR "You can not do in-source compilation. You have to build in a directory distinct from the source directory")
endif()

enable_testing()

# ----------------------------------------------------------------------------

# Shared libraries are mandatory
set (BUILD_SHARED_LIBS TRUE)

# ----------------------------------------------------------------------------

find_package(Arcane REQUIRED)

# ----------------------------------------------------------------------------

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# ----------------------------------------------------------------------------
# Add CSVExample

add_subdirectory(src)
