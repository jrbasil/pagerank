cmake_minimum_required(VERSION 3.24)
project(page_rank)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)
include_directories(test-unit)

add_executable(page_rank src/graph.h test-unit/catch.hpp test-unit/test.cpp)