cmake_minimum_required(VERSION 3.10)

project(htmlanim_docs)

add_executable(htmlanim_docs generate_index.cpp)

target_include_directories(htmlanim_docs PUBLIC ..)
