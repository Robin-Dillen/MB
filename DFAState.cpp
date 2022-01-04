#include "DFAState.h"

#include <utility>

DFAState::DFAState(std::string s, std::list<std::string> c, bool f) : name(std::move(s)), content(std::move(c)), final(f){}

DFAState::~DFAState() {
    transitions.clear();
}

void DFAState::addTransition(const std::string& i, DFAState *state) {
    transitions[i] = state;
}

DFAState *DFAState::getStateOnInput(const std::string& i) {
    if(transitions.find(i) != transitions.end())
        return transitions[i];
    else
        return nullptr;
}

bool DFAState::containsFinalItem() {
    if (content.size() == 1 && content.front().back() == '*')
        return true;
    else
        return false;
}
