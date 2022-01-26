#include "src/Compiler.h"
//#include "src/Data.h"
#include "src/FileLoader.h"
#include "src/Lexer.h"
#include "src/Parser.h"
#include <iostream>
#include <fstream>

int main(int argc, char **argv) {

    FileLoader fl(argv[0]);
    Lexer lexer(fl.getFilecontents());
    const std::vector<Token> &tokens = lexer.getTokens();

    CFG *cfg = new CFG("CFGs/CFG_test.json");
    Parser p(cfg);
    ParseTable table = std::move(p.getParseTable());

    std::ofstream file;
    file.open("ParseTableOutput.txt");
    if (file) {
        table.printTableToFile(file);
    }
    file.close();

    //    compile();

    return 0;
}
