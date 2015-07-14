cmake_minimum_required(VERSION 3.2)
project (xxhashct)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable (xxh64test test.cpp)
