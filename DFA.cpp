#include "DFA.h"

template<typename T>
DFA::DFA(std::vector<T> canSet, const std::list<std::string>& alphabet) {
    //One state per set of LR items
    //Transitions on VAR or TERM based on Closure()
}

DFA::~DFA() {
    while (!states.empty()) {
        DFAState* el = states.back();
        states.pop_back();
        delete el;
    }
}

// adds the state to the dfa if the state is not yet part of the dfa. returns bool with result of addition
bool DFA::addState(DFAState* s) {

    if (this->getState(s->getName()) == nullptr) {
        states.push_back(s);
        return true;
    }
    else
        return false;
}

DFAState *DFA::getState(const std::string& n) const{
    for (auto it : states){
        if(it->getName() == n)
            return it;
    }
    return nullptr;
}

std::map<std::string, std::list<std::string>> DFA::getVarandTerm() const{
    std::map<std::string, std::list<std::string>> result;

    for (DFAState* state : states) {
        for (std::pair<std::string, DFAState*> trans : state->getTransitions()) {
            result["Variables"].push_back(trans.first);
        }
    }

    for (const std::string& el : alphabet) {
        bool found = false;

        for (const std::string& VAR : result["Variables"]) {
            if (VAR == el) {
                found = true;
            }
        }

        if (!found) {
            result["Terminals"].push_back(el);
        }
    }

    return result;
}
