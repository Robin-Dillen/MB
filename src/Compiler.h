#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <marshal.h>
#include <tuple>
#include <iostream>

#include "AbstractSyntaxTree/AbstractSyntaxTree.h"
#include "Data.h"

#ifndef AST_COMPILER_H
#define AST_COMPILER_H

void compile(const AST::AbstractSyntaxTree<Data*>& ast);


#endif //AST_COMPILER_H
