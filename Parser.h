
#ifndef MB_PARSER_H
#define MB_PARSER_H

#include "CFG.h"
#include "CFGVariable.h"
#include "DFA.h"
#include "DFAState.h"
#include <algorithm>

class Parser {
public:
    Parser();
    void printState(map<string, vector<vector<string>>> prods);
private:
    void canonicalCollection();
    map<string, vector<vector<string>>> varsToProds(vector<Variable*> &variables);
    void create_canonical_states(DFAState* rootState);
    int checkSymLoc(vector<string> &production);
    map<string, vector<vector<string>>> goTo(DFAState* rootState, string s);
    void moveLocSym(map<string, vector<vector<string>>> &prods);
    void getClosure(DFAState* rootState, map<string, vector<vector<string>>> &prods);
    void getClosure(DFAState* rootstate, string item, map<string, vector<vector<string>>> &closure);
    DFAState* stateExists(map<string, vector<vector<string>>> &prods);
    CFG* cfg;
    string locSymbole = ".";
    vector<DFAState*> dfaStates;
};


#endif //MB_PARSER_H
