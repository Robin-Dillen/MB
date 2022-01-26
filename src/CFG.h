//
// Created by niels on 12/10/2021.
//

#ifndef PROGRAMMEEROPDRACHT_CFG_H
#define PROGRAMMEEROPDRACHT_CFG_H

#include "CFGVariable.h"

#include <vector>
#include <string>
#include <map>

class CFG {
public:
    explicit CFG(std::string json);

    void print();

    void add_augmented_productions(std::string locSymbol);

    Variable *getStartSymbole() const;

    std::vector<std::string> getTerminals() { return terminals; }

    std::map<std::string, Variable *> getVariables() { return variables; }

    void getClosure(std::string variable, std::vector<Variable *> &closure);

private:
    std::vector<std::string> terminals;
    std::map<std::string, Variable *> variables;
    Variable *startSymbole;
};


#endif //PROGRAMMEEROPDRACHT_CFG_H
