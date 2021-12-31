#include "src/Compiler.h"
#include "src/Data.h"
#include <iostream>

int main(){

    Py_SetPythonHome(L"C:/Users/nibor/AppData/Local/Programs/Python/Python39");
    Py_Initialize();
    Identifier* variable = new Identifier("x");
    AST::AbstractSyntaxTree<Data*> ast_root(new Root);
    AST::AbstractSyntaxTree<Data*> ast_store(new Store);
    AST::AbstractSyntaxTree<Data*> ast_var1(variable);
    AST::AbstractSyntaxTree<Data*> ast_var2(variable);
    AST::AbstractSyntaxTree<Data*> ast_var3(variable);
    AST::AbstractSyntaxTree<Data*> ast_var4(variable);
    AST::AbstractSyntaxTree<Data*> ast_const(new Const(5));
    AST::AbstractSyntaxTree<Data*> ast_while(new While);
    AST::AbstractSyntaxTree<Data*> ast_endwhile(new EndWhile);
    AST::AbstractSyntaxTree<Data*> ast_print(new Print);
    AST::AbstractSyntaxTree<Data*> ast_decr(new Decr);

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

    return 0;
}
