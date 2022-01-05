//
// Created by Khemin on 11-12-2021.
//

#include "ParseTable.h"

ParseTable::ParseTable(const DFA &dfa) {
    Variables = dfa.getVarandTerm()["Variables"];
    Terminals = dfa.getVarandTerm()["Terminals"];

    //Generates the table with empty elements;
    for (DFAState* state : dfa.getStates()) {
        std::map<std::string, std::string> row;
        for (const std::string& t : Terminals) {
            row[t];
        }
        row["EOS"];
        for (const std::string& v : Variables) {
            row[v];
        }
        States.push_back(state->getName());
        table[state->getName()] = row;
    }

    //Fill in table
    for (DFAState* state : dfa.getStates()) {
        for (std::pair<std::string, DFAState*> trans : state->getTransitions()) {
            //state to state on TERM => shift x
            if (foundInStringlist(trans.first, Terminals)) {
                table[state->getName()][trans.first] = "shift " + trans.second->getName();
            }
            //state to state on VAR => GO TO
            if (foundInStringlist(trans.first, Variables)) {
                table[state->getName()][trans.first] = trans.second->getName();
            }
        }
        //state contains final item vb A -> a A * => reduce operation/ write rule
        if (state->containsFinalItem()) {
            if (state->getFinal())
                table[state->getName()]["EOS"] = "accept";
            else {
                std::string reduceRule = state->getContent().back();
                reduceRule.pop_back();
                for (std::pair<std::string, std::string> el: table[state->getName()]) {
                    if (foundInStringlist(el.first, Terminals)) {
                        table[state->getName()][el.first] = reduceRule;
                    }
                }
                table[state->getName()]["EOS"] = reduceRule;
            }
        }
    }
}

std::map<std::string, std::map<std::string, std::string>> ParseTable::getTable() const{
    return table;
}

//void ParseTable::printTable() {
//    std::cout << "     |";
//    for (const std::string& term : Terminals) {
//        std::cout << " " + term + std::string(4-term.size(), ' ') + "|";
//    }
//    std::cout << " EOS |";
//    for (const std::string& var : Variables) {
//        std::cout << " " + var + std::string(4-var.size(), ' ') + "|";
//    }
//    std::cout << '\n';
//    for (std::pair<std::string, std::map<std::string, std::string>> row : table){
//        for (std::pair<std::string, std::string> entry : row.second) {
//
//        }
//    }
//}
