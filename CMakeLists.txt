cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
cmake_policy(VERSION 3.18)

# ---- Project ----

project(
  lu_main
  VERSION 0.0.0
  LANGUAGES C CXX)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()

# ---- Options ----

option(ENABLE_TEST_COVERAGE "Enable test coverage" OFF)

# ---- Dependencies ----

include(cmake/CPM.cmake)

find_package(CrossWindow MODULE REQUIRED)
find_package(lu_base MODULE REQUIRED)

# ---- Add source files ----

# Note: globbing sources is considered bad practice as CMake's generators may not detect new files
# automatically. Keep that in mind when changing files, or explicitly mention them here.
file(GLOB_RECURSE headers CONFIGURE_DEPENDS 
	"${CMAKE_CURRENT_SOURCE_DIR}/source/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
	
if(LU_API STREQUAL "WIN32")
	file(GLOB sources CONFIGURE_DEPENDS 
		"${CMAKE_CURRENT_SOURCE_DIR}/source/Win32Main.cpp")
elseif(LU_API STREQUAL "UWP")
	file(GLOB sources CONFIGURE_DEPENDS 
		"${CMAKE_CURRENT_SOURCE_DIR}/source/UWPMain.cpp")
elseif(LU_API STREQUAL "COCOA")
	file(GLOB sources CONFIGURE_DEPENDS 
		"${CMAKE_CURRENT_SOURCE_DIR}/source/CocoaMain.mm")
elseif(LU_API STREQUAL "UIKIT")
	file(GLOB sources CONFIGURE_DEPENDS 
		"${CMAKE_CURRENT_SOURCE_DIR}/source/UIKitMain.mm")
elseif(LU_API STREQUAL "XCB")
	file(GLOB sources CONFIGURE_DEPENDS 
		"${CMAKE_CURRENT_SOURCE_DIR}/source/XCBMain.cpp")
elseif(LU_API STREQUAL "XLIB")
	file(GLOB sources CONFIGURE_DEPENDS 
		"${CMAKE_CURRENT_SOURCE_DIR}/source/XLibMain.cpp")
elseif(LU_API STREQUAL "ANDROID")
	file(GLOB sources CONFIGURE_DEPENDS 
		"${CMAKE_CURRENT_SOURCE_DIR}/source/AndroidMain.cpp")
elseif(LU_API STREQUAL "WASM")
	file(GLOB sources CONFIGURE_DEPENDS 
		"${CMAKE_CURRENT_SOURCE_DIR}/source/WASMMain.cpp")
elseif(LU_API STREQUAL "NOOP")
	file(GLOB sources CONFIGURE_DEPENDS 
		"${CMAKE_CURRENT_SOURCE_DIR}/source/NoopMain.cpp")
else()
    message( SEND_ERROR "LU_API can only be either AUTO, NOOP, WIN32, UWP, COCOA, UIKIT, XCB, XLIB, MIR, WAYLAND, ANDROID, or WASM.")
endif()

# ---- Target ----

add_library(lu_main ${headers} ${sources})

target_compile_options(lu_main INTERFACE "$<$<C_COMPILER_ID:MSVC>:/utf-8>")
target_compile_options(lu_main INTERFACE "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

set_target_properties(lu_main PROPERTIES CXX_STANDARD 20)

target_compile_definitions(lu_main 
	PUBLIC
		SPDLOG_COMPILED_LIB=1)

# being a cross-platform target, we enforce standards conformance on MSVC
target_compile_options(lu_main PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->")

# Link dependencies
if(LU_PLATFORM_WIN32 OR LU_PLATFORM_UNIVERSAL_WINDOWS)
	target_link_libraries(lu_main PUBLIC 
		userenv)
endif()

target_link_libraries(lu_main PUBLIC CrossWindow lu_base)

target_include_directories(lu_main 
	PUBLIC 
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include/lu_main-0.0.0>
	PRIVATE
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>)

add_library(loopunit::lu_main ALIAS lu_main)

if(ENABLE_TEST_COVERAGE)
  target_compile_options(lu_main PUBLIC -O0 -g -fprofile-arcs -ftest-coverage)
  target_link_options(lu_main PUBLIC -fprofile-arcs -ftest-coverage)
endif()
