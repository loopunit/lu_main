cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
cmake_policy(VERSION 3.18)

# ---- Project ----

project(lu_main_test LANGUAGES CXX)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()

# ---- Dependencies ----

include(../cmake/CPM.cmake)

find_package(CrossWindow MODULE REQUIRED)
find_package(lu_main MODULE REQUIRED)

# ---- Add source files ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

# ---- Target ----

lu_add_executable(lu_main_test ${sources})

target_link_libraries(lu_main_test loopunit::lu_main)

set_target_properties(lu_main_test PROPERTIES CXX_STANDARD 20)
