#ifndef TA_POS_DFASTATE_H
#define TA_POS_DFASTATE_H

#include <string>
#include <vector>
#include <map>
#include <list>

class DFAState {
private:
    std::string name;
    std::map<std::string, std::vector<std::vector<std::string>>> content;
    bool final;
    std::map<std::string, DFAState*> transitions;
public:
    DFAState(std::string s, std::map<std::string, std::vector<std::vector<std::string>>> c, bool f);
    ~DFAState();
    void addTransition(const std::string& i, DFAState* state);
    DFAState* getStateOnInput(const std::string& i);
    std::string getName() const{return name;}
    std::map<std::string, std::vector<std::vector<std::string>>> getContent() const{return content;}
    bool getFinal() const{return final;}
    std::map<std::string, DFAState*> getTransitions() const{return transitions;}
    bool containsFinalItem();
};


#endif //TA_POS_DFASTATE_H
