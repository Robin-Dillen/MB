cmake_minimum_required(VERSION 3.12)
project(MB)

set(CMAKE_CXX_STANDARD 14)

add_executable(MB main.cpp)

find_package(Python COMPONENTS Development REQUIRED)
include_directories ( ${Python_INCLUDE_DIRS} )

add_subdirectory(src)

target_link_libraries(MB PUBLIC Compiler ${Python_LIBRARIES})
