//
// Created by niels on 12/10/2021.
//
#include "CFGVariable.h"

using namespace std;

Variable::Variable(const vector<vector<string>> &productions, const string &name) : productions(productions),
                                                                                    name(name) {}

const string &Variable::getName() const {
    return name;
}

void Variable::deleteProduction(vector<string> prod) {
    for (auto it = productions.begin(); it != productions.end(); it++) {
        if (*it == prod) {
            productions.erase(it);
            break;
        }
    }
}

void Variable::addProduction(vector<string> prod) {
    for (auto it = productions.begin(); it != productions.end(); it++) {
        if (*it == prod) {
            return;
        }
    }
    productions.push_back(prod);
}

const vector<vector<string>> &Variable::getProductions() const {
    return productions;
}

int Variable::countProds() {
    return productions.size();
}

void Variable::editProduction(int indexProds, int indexProd, string change, bool tilEnd, bool replace) {
    if (!replace) {
        vector<string>::iterator it = productions[indexProds].begin() + indexProd;
        productions[indexProds].insert(it, change);
    } else {
        productions[indexProds][indexProd] = change;
    }
    if (tilEnd) {
        while (productions[indexProds].size() != indexProd + 1) {
            productions[indexProds].pop_back();
        }
    }
}

bool Variable::hasProduction(vector<string> a) {
    for (int i = 0; i < productions.size(); i++) {
        if (productions[i].size() != a.size()) continue;
        bool correct = true;
        for (int j = 0; j < productions[i].size(); j++) {
            if (productions[i][j] != a[j]) {
                correct = false;
                break;
            }
        }
        if (correct) {
            return true;
        }
    }
    return false;
}