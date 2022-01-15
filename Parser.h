
#ifndef MB_PARSER_H
#define MB_PARSER_H

#include "CFG.h"
#include "CFGVariable.h"
#include "DFA.h"
#include "DFAState.h"

class Parser {
public:
    Parser();
private:
    void canonicalCollection();
    CFG* cfg;
    string locSymbole = ".";
};


#endif //MB_PARSER_H
