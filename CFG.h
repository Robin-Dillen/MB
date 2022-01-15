//
// Created by niels on 12/10/2021.
//

#ifndef PROGRAMMEEROPDRACHT_CFG_H
#define PROGRAMMEEROPDRACHT_CFG_H

#include <list>
#include <vector>
#include <string>
#include <iostream>
#include "CFGVariable.h"
#include "json.hpp"
#include <fstream>
#include <algorithm>


using namespace std;

class CFG {
public:
    CFG(string json);
    void print();
    void add_augmented_productions(string locSymbol);

    Variable *getStartSymbole() const;

    vector<Variable*> getClosure(string variable);

private:
    vector<string> terminals;
    map<string,Variable*> variables;
    Variable* startSymbole;
};


#endif //PROGRAMMEEROPDRACHT_CFG_H
