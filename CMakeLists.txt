cmake_minimum_required(VERSION 3.19)
project(FO1)

set(CMAKE_CXX_STANDARD 20)

FILE(GLOB files
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

add_executable(FO1 ${files})