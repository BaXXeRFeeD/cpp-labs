cmake_minimum_required(VERSION 3.20)
project(lab9)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab9
        main.cpp PoolAllocator.h)

target_compile_options(lab9 PRIVATE -fexceptions)