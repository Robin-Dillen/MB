#include "src/Compiler.h"
#include "src/Data.h"
#include "src/FileLoader.h"
#include "src/Lexer.h"
#include "src/Parser.h"
#include <iostream>

int main(int argc, char **argv) {

    std::cout << argv[0] << endl;
    FileLoader fl(argv[0]);
    Lexer lexer(fl.getFilecontents());
    const std::vector<Token> &tokens = lexer.getTokens();

    CFG *cfg = new CFG("../CFGs/CFG_2_0.json");
    Parser p(cfg);
    ParseTable table = std::move(p.getParseTable());
//    compile();

    return 0;
}
