cmake_minimum_required(VERSION 3.20)
project(MB)

set(CMAKE_CXX_STANDARD 14)

add_executable(MB main.cpp Compiler.cpp Compiler.h)
find_package( PythonLibs 3.9 REQUIRED )
find_package( PythonInterp 3.9 REQUIRED )
if (PythonLibs_FOUND)
    message("-- python libs found")
endif ()
if (PythonInterp_FOUND)
    message("-- python interp found")
endif ()
include_directories ( ${PYTHON_INCLUDE_DIRS} )

add_library(AST AbstractSyntaxTree.cpp AbstractSyntaxTree.h)
target_link_libraries(MB PUBLIC AST ${PYTHON_LIBRARIES})
