
#ifndef MB_PARSER_H
#define MB_PARSER_H

#include "CFG.h"
#include "CFGVariable.h"
#include "DFA.h"
#include "DFAState.h"
#include "ParseTable.h"
#include <algorithm>

class Parser {
public:
    Parser(CFG* c);
    void printState(map<string, vector<vector<string>>> prods);
    ParseTable getParseTable();
private:
    void canonicalCollection();
    map<string, vector<vector<string>>> varsToProds(const vector<Variable*> &variables);
    void create_canonical_states(DFAState* rootState);
    int checkSymLoc(const vector<string> &production);
    map<string, vector<vector<string>>> goTo(const DFAState* rootState,const string s);
    void moveLocSym(map<string, vector<vector<string>>> &prods);
    void getClosure(const DFAState* rootState, map<string, vector<vector<string>>> &prods);
    void getClosure(const DFAState* rootstate, const string item, map<string, vector<vector<string>>> &closure);
    DFAState* stateExists(const map<string, vector<vector<string>>> &prods);
    CFG* cfg;
    string locSymbole = ".";
    vector<DFAState*> dfaStates;
};


#endif //MB_PARSER_H
