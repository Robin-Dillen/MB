//
// Created by Khemin on 11-12-2021.
//

#ifndef AST_PARSETABLE_H
#define AST_PARSETABLE_H

#include "DFA.h"
#include <map>
#include <iostream>

bool foundInStringlist(const std::string& s, const std::list<std::string>& l){
    bool found = false;
    for (const std::string& el : l) {
        if (el == s){
            found = true;
            break;
        }
    }
    return found;
}

class ParseTable {
    std::list<std::string> Variables;
    std::list<std::string> Terminals;
    std::list<std::string> States;
    std::map<std::string, std::map<std::string, std::string>> table;
public:
    explicit ParseTable(const DFA& dfa);
    std::map<std::string, std::map<std::string, std::string>> getTable() const;
//    void printTable();
};


#endif //AST_PARSETABLE_H
