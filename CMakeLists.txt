cmake_minimum_required(VERSION 3.10)

project(Lab4CUDA)

find_package(CUDA REQUIRED)

cuda_add_executable(
    lab4
    main.cu
)

set(CMAKE_CXX_FLAGS_RELEASE
    "${CMAKE_CXX_FLAGS_RELEASE} /O2"
)

set(CUDA_NVCC_FLAGS_RELEASE
    "${CUDA_NVCC_FLAGS_RELEASE} -O3"
)