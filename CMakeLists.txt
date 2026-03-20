cmake_minimum_required(VERSION 3.10)
project(DA-Project CXX)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(data_structures)

add_executable(program
        main.cpp
)

target_link_libraries(program PRIVATE DataStructures)
