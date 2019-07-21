cmake_minimum_required(VERSION 3.10)

project(htmlanim_docs)

add_executable(htmlanim_docs generate_index.cpp)
target_include_directories(htmlanim_docs PUBLIC ..)

add_executable(ea_demo ea_demo.cpp)
target_include_directories(ea_demo PUBLIC ..)

add_executable(ea_vis1 ea_vis1.cpp)
target_include_directories(ea_vis1 PUBLIC ..)
