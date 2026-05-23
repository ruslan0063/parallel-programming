cmake_minimum_required(VERSION 3.12)

project(lab2_openmp CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP REQUIRED)

add_executable(lab2 main.cpp)

target_link_libraries(lab2 OpenMP::OpenMP_CXX)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(lab2 PRIVATE -Wall -Wextra -O2)
endif()