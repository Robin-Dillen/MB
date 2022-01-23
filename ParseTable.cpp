//
// Created by Khemin on 11-12-2021.
//

#include <fstream>
#include "ParseTable.h"

ParseTable::ParseTable(const DFA &dfa) {
    terminals = dfa.getTerminals();
    variables = vector<std::string>();
    for (const auto& el : dfa.getVariables()) {
        variables.push_back(el.first);
    }

    colWidths["Column1"] = 5;

    //Generates the table with empty elements;
    for (DFAState* state : dfa.getStates()) {
        std::map<std::string, std::string> row;
        for (const std::string& t : terminals) {
            row[t];
            colWidths[t] = t.size() + 2;
        }
        row["EOS"];
        colWidths["EOS"] = 5;
        for (const auto& v : variables) {
            row[v];
            colWidths[v] = v.size() + 2;
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
                if (colWidths[trans.first] < entry.size() + 2)
                    colWidths[trans.first] = entry.size() + 2;
            }
            //state to state on VAR => GO TO
            if (found(trans.first, variables) && trans.first.back() != '\'') {
                std::string entry = trans.second->getName();
                table[state->getName()][trans.first] = entry;
                if (colWidths[trans.first] < entry.size() + 2)
                    colWidths[trans.first] = entry.size() + 2;
            }
        }
        //state contains final item vb A -> a A * => reduce operation/ write rule
        if (state->containsFinalItem(locSymbole)) {
            if (state->getFinal()){
                table[state->getName()]["EOS"] = "accept";
                if (colWidths["EOS"] < 8)
                    colWidths["EOS"] = 8;
            }
            else {
                std::string reduceRule;
                std::vector<string> reduceProd = state->getContent().begin()->second.front();
                reduceRule += state->getContent().begin()->first;
                reduceRule += " -> ";
                for (const std::string& el : reduceProd) {
                    if (el != locSymbole){
                        reduceRule += el + " ";
                    }
                }
                for (std::pair<std::string, std::string> el : table[state->getName()]) {
                    if (found(el.first, terminals)) {
                        table[state->getName()][el.first] = reduceRule;
                        if (colWidths[el.first] < reduceRule.size() + 1)
                            colWidths[el.first] = reduceRule.size() + 1;
                    }
                }
                if (table[state->getName()]["EOS"].empty()){
                    table[state->getName()]["EOS"] = reduceRule;
                    if (colWidths["EOS"] < reduceRule.size() + 1)
                        colWidths["EOS"] = reduceRule.size() + 1;
                }
            }
        }
    }
}

std::map<std::string, std::map<std::string, std::string>> ParseTable::getTable() const{
    return table;
}

void ParseTable::printTableToFile(std::ofstream& out) {
    std::string lineString;
    out << std::string(colWidths["Column1"], ' ') + "|";
    lineString += std::string(colWidths["Column1"], '_') + "|";
    for (std::string& t : terminals) {
        out << " " + t + std::string(colWidths[t] - t.size() - 1, ' ') + "|";
        lineString += std::string(colWidths[t], '_') + "|";
    }
    out << " EOS" + std::string(colWidths["EOS"] - 4, ' ') + "|";
    lineString += std::string(colWidths["EOS"], '_') + "|";
    for (std::string& v: variables) {
        if (v.back() != '\''){
            out << " " + v + std::string(colWidths[v] - v.size() - 1, ' ') + "|";
            lineString += std::string(colWidths[v], '_') + "|";
        }
    }
    out << std::endl;
    out << lineString << std::endl;

    for (auto& row : table) {
        out << " " + row.first + std::string(colWidths["Column1"] - row.first.size() - 1, ' ') + "|";

        for (std::string& col : terminals){
            out << " " + row.second.at(col) + std::string(colWidths[col] - row.second.at(col).size() - 1, ' ') + "|";
        }
        out << " " + row.second.at("EOS") + std::string(colWidths["EOS"] - row.second.at("EOS").size() - 1, ' ') + "|";
        for (std::string& col : variables){
            if (col.back() != '\'') {
                out << " " + row.second.at(col) + std::string(colWidths[col] - row.second.at(col).size() - 1, ' ') + "|";
            }
        }
        out << std::endl;
        out << lineString << std::endl;
    }
}
