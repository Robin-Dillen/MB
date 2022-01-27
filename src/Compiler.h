#define PY_SSIZE_T_CLEAN

#include "AbstractSyntaxTree/AbstractSyntaxTree.h"
#include "lib.h"
#include "Data.h"

#ifndef AST_COMPILER_H
#define AST_COMPILER_H

class CompilationError : public std::runtime_error {
public:
    explicit CompilationError(const std::string &arg) : runtime_error(arg) {
    }

    ~CompilationError() noexcept override = default;
};

void compile(const AST::AbstractSyntaxTree<Token*> &ast, const std::string& filename);


#endif //AST_COMPILER_H
