cmake_minimum_required(VERSION 4.2)
project(fun_using_computational_chemistry)

set(CMAKE_CXX_STANDARD 26)

# --- Eigen ---
find_package(Eigen3 REQUIRED NO_MODULE)

# --- inputlib ---
add_library(inputlib STATIC
        src/inputlib/readinp.cpp
        src/inputlib/readinp.hpp
        src/inputlib/readmat.cpp
        src/inputlib/readmat.hpp
)
target_include_directories(inputlib PUBLIC src/inputlib)

target_link_libraries(inputlib PUBLIC Eigen3::Eigen)

# --- Hückel executable ---
add_executable(fun_using_computational_chemistry
        src/huckellib/huckel_driver.cpp
        src/huckellib/diagmat.hpp
        src/huckellib/diagmat.cpp
)
target_link_libraries(fun_using_computational_chemistry PRIVATE
        inputlib
        Eigen3::Eigen
)
set_target_properties(fun_using_computational_chemistry PROPERTIES
        OUTPUT_NAME "huckel"
)