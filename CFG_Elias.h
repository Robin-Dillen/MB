//
// Created by elias  on 10/10/2021.
//

#ifndef MB_PROGRAMMEEROPDRACHTEN_CFG_H
#define MB_PROGRAMMEEROPDRACHTEN_CFG_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cmath>
#include <algorithm>
#include "json.hpp"

using namespace std;

using json = nlohmann::json;

class CFG_Elias {
private:
    void readfile(const string &inputfile);
    vector<string> V; // nonterminals (symbols used for the production rules)
    vector<string> T; // terminals (symbols used for the strings)
    string startsymbol;
    map<string, vector<vector<string>>> production_rules;

public:

    CFG_Elias(const string &inputfile);

    void print();

    void add_augmented_productions(string locSymbol);

    map<string, vector<vector<string>>> getClosure(string variable);

    string getStartSymbol();

    CFG_Elias(const vector<string> &v, const vector<string> &t, const string &startsymbol,
        const map<string, vector<vector<string>>> &productionRules);

    void toCNF();


};


#endif //MB_PROGRAMMEEROPDRACHTEN_CFG_H
