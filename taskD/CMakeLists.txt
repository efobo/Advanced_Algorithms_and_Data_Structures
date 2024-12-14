cmake_minimum_required(VERSION 3.29)
project(TaskD)

set(CMAKE_CXX_STANDARD 20)
set(CXXFLAGS="-std=c++20 -Wall -Wextra -Wpedantic -Werror -g")

add_executable(TaskD main.cpp)
