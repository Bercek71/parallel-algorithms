cmake_minimum_required(VERSION 3.29)
project(Task_1)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenMP REQUIRED)

add_executable(Task_1 main.cpp)

if (OPENMP_CXX_FOUND)
    target_link_libraries(Task_1 PUBLIC OpenMP::OpenMP_CXX)
endif()
