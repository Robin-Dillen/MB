add_library(Compiler Compiler.cpp Compiler.h lib.h Data.cpp Data.h)

add_subdirectory(AbstractSyntaxTree)
target_link_libraries(Compiler AST)