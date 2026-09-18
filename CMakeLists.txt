cmake_minimum_required(VERSION 4.2)
project(fun_using_computational_chemistry)

set(CMAKE_CXX_STANDARD 26)

add_executable(fun_using_computational_chemistry src/main.cpp
        src/inputlib/readinp.hpp
        src/inputlib/readinp.cpp)
