#include "src/Compiler.h"
#include "src/Data.h"
#include <iostream>
#include "Parser.h"

int main(){
    Parser p;
    ParseTable table = p.getParseTable();
    table.printTable();


//    Py_SetPythonHome(L"C:/Users/nibor/AppData/Local/Programs/Python/Python39");
/*
    Py_Initialize();
    Identifier* variable = new Identifier("x");
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

    Py_Finalize();
*/
    return 0;
}
