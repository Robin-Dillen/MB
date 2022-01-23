#include "DFAState.h"

#include <utility>

DFAState::DFAState(std::string s, std::map<std::string, std::vector<std::vector<std::string>>> c, bool f) : name(std::move(s)), content(std::move(c)){
    if (content.size() == 1 && content.begin()->first.back() == '\'' && content.begin()->second.size() == 1 && content.begin()->second.front().back() == ".")
        final = true;
    else
        final = f;
}

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

bool DFAState::containsFinalItem(const std::string& locSymb) {
    if (content.size() == 1) {
        for (const auto& prods : content)
            if (prods.second.size() == 1 && prods.second.front().back() == locSymb)
                return true;
    }

    return false;
}
