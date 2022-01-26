add_library(Compiler
        CFG.cpp CFG.h
        CFGVariable.cpp CFGVariable.h
        Compiler.cpp Compiler.h
        DFA.cpp DFA.h
        DFAState.cpp DFAState.h
        FileLoader.cpp FileLoader.h
        Lexer.cpp Lexer.h
        lib.h
        Parser.cpp Parser.h
        ParseTable.cpp ParseTable.h)

add_subdirectory(AbstractSyntaxTree)
add_subdirectory(json)

target_link_libraries(Compiler AST DFA JSON)