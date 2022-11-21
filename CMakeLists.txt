cmake_minimum_required(VERSION 3.16)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt. ")
endif()

project(smem)

set(VERSION_MAJOR 0 CACHE STRING "Project major version number.")
set(VERSION_MINOR 1 CACHE STRING "Project minor version number.")
set(VERSION_PATCH 2 CACHE STRING "Project patch version number.")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(SMEM_HEADER_ONLY "SMEM_HEADER_ONLY" ON)
option(SMEM_BUILD_EXAMPLE "SMEM_BUILD_EXAMPLE" ON)
option(SMEM_ENABLE_TEST "SMEM_ENABLE_TEST" OFF)

if(MSVC)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

if(SMEM_HEADER_ONLY)
  add_library(smem INTERFACE)
  target_include_directories(smem INTERFACE ${PROJECT_SOURCE_DIR}/include)
  target_compile_definitions(smem INTERFACE SMEM_HEADER_ONLY)
else()
  add_library(smem ${PROJECT_SOURCE_DIR}/include/smem/smem.hpp ${PROJECT_SOURCE_DIR}/include/smem/impl/smem.cpp)
  target_include_directories(smem PUBLIC ${PROJECT_SOURCE_DIR}/include)
endif()

if(SMEM_BUILD_EXAMPLE)
  add_subdirectory(examples)
endif()

if(SMEM_ENABLE_TEST)
  cmake_policy(SET CMP0077 NEW)
  if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
  endif()
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
  )
  set(BUILD_GMOCK OFF)
  set(INSTALL_GTEST OFF)
  if(WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  endif()
  FetchContent_MakeAvailable(googletest)
  add_subdirectory(tests)
endif()
