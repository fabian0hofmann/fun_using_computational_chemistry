cmake_minimum_required(VERSION 4.2)
project(fun_using_computational_chemistry)

set(CMAKE_CXX_STANDARD 26)

# --- inputlib ---
add_library(inputlib STATIC
        src/inputlib/readinp.cpp
        src/inputlib/readinp.hpp
)
target_include_directories(inputlib PUBLIC src/inputlib)

# --- main executable ---
add_executable(fun_using_computational_chemistry
        src/main.cpp
)
target_link_libraries(fun_using_computational_chemistry PRIVATE
        inputlib
)