#define PY_SSIZE_T_CLEAN

#include "AbstractSyntaxTree/AbstractSyntaxTree.h"
#include "lib.h"
#include "Data.h"

#ifndef AST_COMPILER_H
#define AST_COMPILER_H

void compile(const AST::AbstractSyntaxTree<Token*> &ast);


#endif //AST_COMPILER_H
