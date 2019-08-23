cmake_minimum_required(VERSION 3.0)
project(slotmap)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -pedantic -Werror")

add_executable(slotmap_test main.cpp test.cpp)
