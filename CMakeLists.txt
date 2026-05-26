cmake_minimum_required(VERSION 3.12)

project("lab5_supercomputer" CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(MPI REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} MPI::MPI_CXX)