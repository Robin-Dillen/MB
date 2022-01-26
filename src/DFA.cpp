#include "DFA.h"

#include <utility>
#include <list>

using namespace std;

DFA::DFA(const std::vector<DFAState *> &stateSet, const vector<std::string> &terms,
         const map<std::string, Variable *> &vars) : terminals(terms), variables(vars) {
    states = std::vector<DFAState *>();
    for (DFAState *state: stateSet) {
        states.push_back(state);
        if (state->getName() == "IO") {
            startState = state;
        }
    }
    currentState = startState;
}

DFA::~DFA() {
    while (!states.empty()) {
        DFAState *el = states.back();
        states.pop_back();
        delete el;
    }
}

// adds the state to the dfa if the state is not yet part of the dfa. returns bool with result of addition
bool DFA::addState(DFAState *s) {

    if (this->getState(s->getName()) == nullptr) {
        states.push_back(s);
        return true;
    } else
        return false;
}

DFAState *DFA::getState(const std::string &n) const {
    for (auto it: states) {
        if (it->getName() == n)
            return it;
    }
    return nullptr;
}
