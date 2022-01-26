#ifndef TA_POS_DFA_H
#define TA_POS_DFA_H

#include <string>
#include <utility>
#include <vector>
#include <list>
#include "DFAState.h"
#include "CFGVariable.h"

class DFAState;

class DFA {
private:
    DFAState *currentState;
    DFAState *startState;
    std::vector<DFAState *> states;
    vector <std::string> terminals;
    map<std::string, Variable *> variables;
public:
    DFA() {
        currentState = nullptr;
        startState = nullptr;
        states = std::vector<DFAState *>();
    };

    DFA(const std::vector<DFAState *> &stateSet, const vector <std::string> &terms,
        const map<std::string, Variable *> &vars);

    ~DFA();

    bool addState(DFAState *s);

    DFAState *getState(const std::string &n) const;

    DFAState *getCurrentState() const { return currentState; };

    void setCurrentState(DFAState *s) { currentState = s; };

    DFAState *getStartState() const { return startState; };

    void setStartState(DFAState *s) { startState = s; };

    vector <string> getTerminals() const { return terminals; }

    map<string, Variable *> getVariables() const { return variables; }

    std::vector<DFAState *> getStates() const { return states; };
};


#endif //TA_POS_DFA_H
