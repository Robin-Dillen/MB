//
// Created by Khemin on 11-12-2021.
//

#include "ParseTable.h"

ParseTable::ParseTable(const DFA &dfa) {
    terminals = dfa.getTerminals();
    variables = vector<std::string>();
    for (const auto& el : dfa.getVariables()) {
        variables.push_back(el.first);
    }

    //Generates the table with empty elements;
    for (DFAState* state : dfa.getStates()) {
        std::map<std::string, std::string> row;
        for (const std::string& t : terminals) {
            row[t];
        }
        row["EOS"];
        for (const auto& v : variables) {
            row[v];
        }
        States.push_back(state->getName());
        table[state->getName()] = row;
    }

    //Fill in table
    for (DFAState* state : dfa.getStates()) {
        for (std::pair<std::string, DFAState*> trans : state->getTransitions()) {
            //state to state on TERM => shift x
            if (found(trans.first, terminals)) {
                std::string entry = "shift " + trans.second->getName();
                table[state->getName()][trans.first] = entry;
                if (colWidth < entry.size() + 2)
                    colWidth = entry.size() + 2;
            }
            //state to state on VAR => GO TO
            if (found(trans.first, variables)) {
                std::string entry = trans.second->getName();
                table[state->getName()][trans.first] = entry;
                if (colWidth < entry.size() + 2)
                    colWidth = entry.size() + 2;
            }
        }
        //state contains final item vb A -> a A * => reduce operation/ write rule
        if (state->containsFinalItem(locSymbole)) {
            if (state->getFinal()){
                table[state->getName()]["EOS"] = "accept";
                if (colWidth < 8)
                    colWidth = 8;
            }

            else {
                //TODO
                //std::string reduceRule = state->getContent().back();
                std::string reduceRule = "todo";
                reduceRule.pop_back();
                for (std::pair<std::string, std::string> el: table[state->getName()]) {
                    if (found(el.first, terminals)) {
                        table[state->getName()][el.first] = reduceRule;
                        if (colWidth < reduceRule.size() + 2)
                            colWidth = reduceRule.size() + 2;
                    }
                }
                if (table[state->getName()]["EOS"].empty())
                    table[state->getName()]["EOS"] = reduceRule;
            }
        }
    }
}

std::map<std::string, std::map<std::string, std::string>> ParseTable::getTable() const{
    return table;
}

void ParseTable::printTable() {
    std::string lineString;
    std::cout << std::string(colWidth, ' ') + "|";
    lineString += std::string(colWidth, '_') + "|";
    for (const std::string& t : terminals) {
        std::cout << " " + t + std::string(colWidth - t.size() - 1, ' ') + "|";
        lineString += std::string(colWidth, '_') + "|";
    }
    for (const std::string& v: variables) {
        std::cout << " " + v + std::string(colWidth - v.size() - 1, ' ') + "|";
        lineString += std::string(colWidth, '_') + "|";
    }
    std::cout << "\n";
    lineString += "\n";
    std::cout << lineString;

    for (const auto& row : table) {
        std::cout << " " + row.first + std::string(colWidth - row.first.size() - 1, ' ') + "|";
        for (const auto& entry : row.second) {
            std::cout << " " + entry.second + std::string(colWidth - row.first.size() - 1, ' ') + "|";
        }
        std::cout << "\n";
        std::cout << lineString;
    }
}
