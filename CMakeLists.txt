cmake_minimum_required (VERSION 2.6)
project (work)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory (c++)
add_subdirectory (sim)
