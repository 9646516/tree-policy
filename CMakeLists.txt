cmake_minimum_required(VERSION 3.21)
project(tree_policy)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

include_directories(PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests)

add_executable(test_rand tests/test_rand.cpp)
add_executable(test_desc tests/test_desc.cpp)
add_executable(test_asc tests/test_asc.cpp)
