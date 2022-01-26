//
// Created by niels on 12/10/2021.
//

#include "CFG.h"
#include "json/json.hpp"

#include <list>
#include <iostream>
#include <fstream>
#include <algorithm>

using json = nlohmann::json;
using namespace std;

//constructor
CFG::CFG(string jsonfile) {
    ifstream input(jsonfile);
    if (jsonfile.empty()) return;
    json j;
    input >> j;
    terminals = j["Terminals"].get<vector<string>>();
    vector<string> j_variables = j["Variables"].get<vector<string>>();
    vector<json> j_productions = j["Productions"].get<vector<json>>();
    string j_start = j["Start"].get<string>();

    vector<string> variable_strings;
    variable_strings.reserve(j_variables.size());
    for (int i = 0; i < j_variables.size(); i++) {
        variable_strings.push_back(j_variables[i]);
    }
    sort(variable_strings.begin(), variable_strings.end());
    for (int i = 0; i < variable_strings.size(); i++) {
        vector<vector<string>> productions;
        for (int k = 0; k < j_productions.size(); k++) {
            if (variable_strings[i] == j_productions[k]["head"].get<string>()) {
                productions.push_back(j_productions[k]["body"].get<vector<string>>());
            }
        }
        std::sort(productions.begin(), productions.end());
        Variable *v = new Variable(productions, variable_strings[i]);
        variables[variable_strings[i]] = v;
        if (variable_strings[i] == j_start) startSymbole = v;
    }


    sort(terminals.begin(), terminals.end());
}

//print function that prints a CFG to the console
void CFG::print() {
    cout << "V = {";
    for (auto it = variables.begin(); it != variables.end(); it++) {
        if (it != variables.begin()) {
            cout << ", ";
        }
        cout << it->second->getName();
    }
    cout << "}" << endl;
    cout << "T = {";
    for (int i = 0; i < terminals.size(); i++) {
        cout << terminals[i];
        if (i != terminals.size() - 1) {
            cout << ", ";
        }
    }
    cout << "}" << endl;
    cout << "P = {" << endl;
    for (auto it = variables.begin(); it != variables.end(); it++) {
        vector<vector<string>> productions = it->second->getProductions();
        for (int j = 0; j < productions.size(); j++) {
            string production;
            for (int k = 0; k < productions[j].size(); k++) {
                production += productions[j][k];
                if (k != productions[j].size() - 1) {
                    production += " ";
                }
            }
            cout << "  " << it->second->getName() << " -> `" << production << "`" << endl;
        }
    }
    cout << "}" << endl;
    cout << "S = " << startSymbole->getName() << endl;
}

void CFG::add_augmented_productions(string locSymbol) {
    /// we add a new starting Variable
    string new_start_name = startSymbole->getName() + "'";
    vector<string> production = {startSymbole->getName()};
    Variable *v = new Variable({production}, new_start_name);
    variables[new_start_name] = v;
    startSymbole = v;

    // making an augmented production by adding • to all the productions
    for (auto it = variables.begin(); it != variables.end(); it++) {
        vector<vector<string>> productions = it->second->getProductions();
        for (int j = 0; j < productions.size(); j++) {
            it->second->editProduction(j, 0, locSymbol, false, false);
        }
    }
}

Variable *CFG::getStartSymbole() const {
    return startSymbole;
}

void CFG::getClosure(string variable, vector<Variable *> &closure) {
    if (variables.find(variable) == variables.end()) return;
    if (closure.empty()) closure.push_back(variables[variable]);

    vector<vector<string>> prods = variables[variable]->getProductions();
    for (auto it1 = prods.begin(); it1 != prods.end(); it1++) {
        vector<string> prod = *it1;
        for (auto it2 = prod.begin(); it2 != prod.end(); it2++) {
            if (variables.find(*it2) == variables.end()) continue;
            bool found = false;
            for (int i = 0; i < closure.size(); i++) {
                if (closure[i]->getName() == *it2) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                closure.push_back(variables[*it2]);
                getClosure(*it2, closure);
            }
        }
    }
    return;
}
