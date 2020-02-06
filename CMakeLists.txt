cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_FLAGS "-std=c++17 -g -mavx -msse4.1 -Wpsabi -march=native")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse4.1 -maes -mavx")

add_executable(POSTests "main_tests.cpp")

