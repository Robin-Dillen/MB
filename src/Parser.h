
#ifndef MB_PARSER_H
#define MB_PARSER_H

#include "CFG.h"
#include "CFGVariable.h"
#include "DFA.h"
#include "DFAState.h"
#include "ParseTable.h"

#include <string>
#include <vector>
#include <map>

class Parser {
public:
    Parser(CFG *c);

    void printState(std::map<std::string, std::vector<std::vector<std::string>>> prods);

    ParseTable getParseTable();

private:
    void canonicalCollection();

    std::map<std::string, std::vector<std::vector<std::string>>> varsToProds(const std::vector<Variable *> &variables);

    void create_canonical_states(DFAState *rootState);

    int checkSymLoc(const std::vector<std::string> &production);

    std::map<std::string, std::vector<std::vector<std::string>>> goTo(const DFAState *rootState, std::string s);

    void moveLocSym(std::map<std::string, std::vector<std::vector<std::string>>> &prods);

    void getClosure(std::map<std::string, std::vector<std::vector<std::string>>> &prods);

    void getClosure(std::string item, std::map<std::string, std::vector<std::vector<std::string>>> &closure);

    DFAState *stateExists(const std::map<std::string, std::vector<std::vector<std::string>>> &prods);

    CFG *cfg;
    std::string locSymbole{"."};
    std::vector<DFAState *> dfaStates;
    DFAState* UpperRoot;
};


#endif //MB_PARSER_H
