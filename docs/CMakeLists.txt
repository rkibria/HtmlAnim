cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(htmlanim_docs)

add_executable(htmlanim_docs generate_index.cpp)
target_include_directories(htmlanim_docs PUBLIC ..)

add_executable(offscreens offscreens.cpp)
target_include_directories(offscreens PUBLIC ..)
