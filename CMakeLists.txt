cmake_minimum_required(VERSION 3.12)
project(MB)

set(CMAKE_CXX_STANDARD 14)


add_executable(MB main.cpp)

find_package( PythonLibs 3.9 REQUIRED )
find_package( PythonInterp 3.9 REQUIRED )
if (PythonLibs_FOUND)
    message("-- python libs found")
endif ()
if (PythonInterp_FOUND)
    message("-- python interp found")
endif ()
include_directories ( ${PYTHON_INCLUDE_DIRS} )

add_subdirectory(src)

target_link_libraries(MB PUBLIC Compiler ${PYTHON_LIBRARIES})
