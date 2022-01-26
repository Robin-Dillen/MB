#ifndef TA_POS_DFA_H
#define TA_POS_DFA_H

#include <vector>
#include <string>
#include "DFAState.h"
#include "CFGVariable.h"

class DFA {
private:
    DFAState *currentState;
    DFAState *startState;
    std::vector<DFAState *> states;
    std::vector<std::string> terminals;
    std::map<std::string, Variable *> variables;
public:
    DFA() {
        currentState = nullptr;
        startState = nullptr;
        states = std::vector<DFAState *>();
    };

    DFA(const std::vector<DFAState *> &stateSet, const std::vector<std::string> &terms,
        const std::map<std::string, Variable *> &vars);

    ~DFA();

    bool addState(DFAState *s);

    DFAState *getState(const std::string &n) const;

    DFAState *getCurrentState() const { return currentState; };

    void setCurrentState(DFAState *s) { currentState = s; };

    DFAState *getStartState() const { return startState; };

    void setStartState(DFAState *s) { startState = s; };

    std::vector<std::string> getTerminals() const { return terminals; }

    std::map<std::string, Variable *> getVariables() const { return variables; }

    std::vector<DFAState *> getStates() const { return states; };
};


#endif //TA_POS_DFA_H
