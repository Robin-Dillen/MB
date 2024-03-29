//
// Created by Khemin on 11-12-2021.
//

#ifndef AST_PARSETABLE_H
#define AST_PARSETABLE_H

#include "DFA.h"
#include "Lexer.h"
#include "AbstractSyntaxTree/AbstractSyntaxTree.h"

#include <map>
#include <iostream>


inline bool found(const std::string &s, const std::vector<std::string> &vec) {
    bool found = false;
    for (const std::string &el: vec) {
        if (el == s) {
            found = true;
            break;
        }
    }
    return found;
}

class ParseTable {
    std::vector<std::string> terminals;
    std::vector<std::string> variables;
    std::list<std::string> States;
    std::map<std::string, std::map<std::string, std::string>> table;
    std::string locSymbole = ".";
    std::map<std::string, unsigned int> colWidths;

    bool computeOperation(std::vector<std::string> &contents, std::vector<Token> &remainingInput, const std::string &operation, std::string token = "EOS");
public:
    explicit ParseTable(const DFA &dfa);

    std::map<std::string, std::map<std::string, std::string>> getTable() const;

    void printTableToFile(std::ofstream& out);

    AST::AbstractSyntaxTree<Token*>* checkInputTokens(const std::vector<Token>& input);
};


#endif //AST_PARSETABLE_H
