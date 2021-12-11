#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <marshal.h>
#include <tuple>
#include "AbstractSyntaxTree.h"

#ifndef AST_COMPILER_H
#define AST_COMPILER_H

template <typename T>
std::tuple<std::string, int> compile(const AST::AbstractSyntaxTree<T>& ast);
#endif //AST_COMPILER_H
