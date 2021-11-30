cmake_minimum_required(VERSION 3.20)
project(AST)

set(CMAKE_CXX_STANDARD 14)

add_executable(AST main.cpp)

add_library(test AbstractSyntaxTree.cpp AbstractSyntaxTree.h)
target_link_libraries(AST PRIVATE test)
