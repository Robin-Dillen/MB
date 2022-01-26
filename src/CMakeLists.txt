add_library(Compiler
        CFG.cpp CFG.h
        CFGVariable.cpp CFGVariable.h
        Compiler.cpp Compiler.h
        FileLoader.cpp FileLoader.h
        Lexer.cpp Lexer.h
        lib.h
        Parser.cpp Parser.h)

add_subdirectory(AbstractSyntaxTree)
add_subdirectory(DFA)
add_subdirectory(json)

target_link_libraries(Compiler AST DFA JSON)