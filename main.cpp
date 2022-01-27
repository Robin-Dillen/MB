#include "src/Compiler.h"
//#include "src/Data.h"
#include "src/FileLoader.h"
#include "src/Lexer.h"
#include "src/Parser.h"
#include <iostream>
#include <fstream>

int main(int argc, char **argv) {

    FileLoader fl("../testcode.txt");
    Lexer lexer(fl.getFilecontents());
    const std::vector<Token> &tokens = lexer.getTokens();

    for(auto i : tokens){
        std::cout<<i.value<< std::endl;
    }

    CFG *cfg = new CFG("../CFGs/CFG_2_0.json");
    cfg->print();

    Parser p(cfg);
    ParseTable table = std::move(p.getParseTable());


    std::ofstream file;
    file.open("../ParseTableOutput.txt");
    if (file) {
        table.printTableToFile(file);
    }
    file.close();

    table.checkInputTokens(tokens);




    /*Identifier* variable = new Identifier("x");
    AST::AbstractSyntaxTree<Data*> ast_root(new Root, 0);
    AST::AbstractSyntaxTree<Data*> ast_store(new Store, 0);
    AST::AbstractSyntaxTree<Data*> ast_var1(variable, 0);
    AST::AbstractSyntaxTree<Data*> ast_var2(variable, 0);
    AST::AbstractSyntaxTree<Data*> ast_var3(variable, 0);
    AST::AbstractSyntaxTree<Data*> ast_var4(variable, 0);
    AST::AbstractSyntaxTree<Data*> ast_const(new Const(5), 0);
    AST::AbstractSyntaxTree<Data*> ast_while(new While, 0);
    AST::AbstractSyntaxTree<Data*> ast_endwhile(new End, 0);
    AST::AbstractSyntaxTree<Data*> ast_print(new Print, 0);
    AST::AbstractSyntaxTree<Data*> ast_decr(new Decr, 0);

    ast_root.appendChild(&ast_store);
    ast_root.appendChild(&ast_while);

    ast_store.appendChild(&ast_var1);
    ast_store.appendChild(&ast_const);

    ast_while.appendChild(&ast_var2);
    ast_while.appendChild(&ast_print);
    ast_while.appendChild(&ast_decr);
    ast_while.appendChild(&ast_endwhile);

    ast_print.appendChild(&ast_var3);

    ast_decr.appendChild(&ast_var4);

    compile(ast_root);
    */

    return 0;
}
