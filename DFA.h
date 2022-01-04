#ifndef TA_POS_DFA_H
#define TA_POS_DFA_H

#include <string>
#include <utility>
#include <vector>
#include <list>
#include "DFAState.h"

class DFAState;
class DFA {
private:
    DFAState* currentState;
    DFAState* startState;
    std::vector<DFAState*> states;
    std::list<std::string> alphabet;
public:
    DFA(){ currentState = nullptr; startState = nullptr; states = std::vector<DFAState*>();};
    template<typename T> DFA(std::vector<T> canSet, const std::list<std::string>& alphabet);
    ~DFA();

    bool addState(DFAState* s);

    DFAState* getState(const std::string& n) const;
    DFAState* getCurrentState() const{return currentState;};
    void setCurrentState(DFAState* s){ currentState = s;};
    DFAState* getStartState(){return startState;};
    void setStartState(DFAState* s){ startState = s;};
    std::list<std::string> getAlphabet(){return alphabet;};
    void setAlphabet(std::list<std::string> l){alphabet = std::move(l);};
    std::vector<DFAState*> getStates() const{return states;};
    std::map<std::string, std::list<std::string>> getVarandTerm() const;
};


#endif //TA_POS_DFA_H
