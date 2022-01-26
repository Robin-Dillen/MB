//
// Created by niels on 12/10/2021.
//

#ifndef PROGRAMMEEROPDRACHT_VARIABLE_H
#define PROGRAMMEEROPDRACHT_VARIABLE_H

#include <vector>
#include <string>

using namespace std;

class Variable {
public:
    Variable(const vector<vector<string>> &productions, const string &name);
    const string &getName() const;

    void deleteProduction(vector<string> prod);
    void addProduction(vector<string> prod);
    const vector<vector<string>> &getProductions() const;
    int countProds();
    void editProduction(int indexProds, int indexProd, string change, bool tilend = false, bool replace = true);
    bool hasProduction(vector<string> a);

private:
    vector<vector<string>> productions;
    string name;
};


#endif //PROGRAMMEEROPDRACHT_VARIABLE_H
