//
// Created by Khemin on 11-12-2021.
//
#include "ParseTable.h"
#include "AbstractSyntaxTree/AbstractSyntaxTree.h"
#include "Data.h"

#include <fstream>
#include <algorithm>
#include <stack>


ParseTable::ParseTable(const DFA &dfa) {
    terminals = dfa.getTerminals();
    variables = std::vector<std::string>();
    for (const auto &el: dfa.getVariables()) {
        variables.push_back(el.first);
    }

    colWidths["Column1"] = 5;

    //Generates the table with empty elements;
    for (DFAState *state: dfa.getStates()) {
        std::map<std::string, std::string> row;
        for (const std::string &t: terminals) {
            row[t];
            colWidths[t] = t.size() + 2;
        }
        row["EOS"];
        colWidths["EOS"] = 5;
        for (const auto &v: variables) {
            row[v];
            colWidths[v] = v.size() + 2;
        }
        States.push_back(state->getName());
        table[state->getName()] = row;
    }

    //Fill in table
    for (DFAState *state: dfa.getStates()) {
        for (std::pair<std::string, DFAState *> trans: state->getTransitions()) {
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
            if (state->getFinal()) {
                table[state->getName()]["EOS"] = "accept";
                if (colWidths["EOS"] < 8)
                    colWidths["EOS"] = 8;
            } else {
                std::string reduceRule;
                std::vector<std::string> reduceProd = state->getContent().begin()->second.front();
                reduceRule += state->getContent().begin()->first;
                reduceRule += " -> ";
                for (const std::string &el: reduceProd) {
                    if (el != locSymbole) {
                        reduceRule += el + " ";
                    }
                }
                for (std::pair<std::string, std::string> el: table[state->getName()]) {
                    if (found(el.first, terminals)) {
                        table[state->getName()][el.first] = reduceRule;
                        if (colWidths[el.first] < reduceRule.size() + 1)
                            colWidths[el.first] = reduceRule.size() + 1;
                    }
                }
                if (table[state->getName()]["EOS"].empty()) {
                    table[state->getName()]["EOS"] = reduceRule;
                    if (colWidths["EOS"] < reduceRule.size() + 1)
                        colWidths["EOS"] = reduceRule.size() + 1;
                }
            }
        }
    }
}

std::map<std::string, std::map<std::string, std::string>> ParseTable::getTable() const {
    return table;
}

void ParseTable::printTableToFile(std::ofstream &out) {
    std::string lineString;
    out << std::string(colWidths["Column1"], ' ') + "|";
    lineString += std::string(colWidths["Column1"], '_') + "|";
    for (std::string &t: terminals) {
        out << " " + t + std::string(colWidths[t] - t.size() - 1, ' ') + "|";
        lineString += std::string(colWidths[t], '_') + "|";
    }
    out << " EOS" + std::string(colWidths["EOS"] - 4, ' ') + "|";
    lineString += std::string(colWidths["EOS"], '_') + "|";
    for (std::string &v: variables) {
        if (v.back() != '\'') {
            out << " " + v + std::string(colWidths[v] - v.size() - 1, ' ') + "|";
            lineString += std::string(colWidths[v], '_') + "|";
        }
    }
    out << std::endl;
    out << lineString << std::endl;

    for (auto &row: table) {
        out << " " + row.first + std::string(colWidths["Column1"] - row.first.size() - 1, ' ') + "|";

        for (std::string &col: terminals) {
            out << " " + row.second.at(col) + std::string(colWidths[col] - row.second.at(col).size() - 1, ' ') + "|";
        }
        out << " " + row.second.at("EOS") + std::string(colWidths["EOS"] - row.second.at("EOS").size() - 1, ' ') + "|";
        for (std::string &col: variables) {
            if (col.back() != '\'') {
                out << " " + row.second.at(col) + std::string(colWidths[col] - row.second.at(col).size() - 1, ' ') +
                       "|";
            }
        }
        out << std::endl;
        out << lineString << std::endl;
    }
}

std::string getTypeString(enum TokenType type){
    switch(type) {
        case while_ :
            return "while";
        case incr_ :
            return "incr";
        case decr_ :
            return "decr";
        case print_ :
            return "print";
        case import_ :
            return "import";
        case identifier_ :
            return "identifier";
        case number_ :
            return "number";
        case operator_ :
            return "operator";
        case lparen_ :
            return "lparen";
        case rparen_ :
            return "rparen";
        case lbrace_ :
            return "lbrace";
        case rbrace_ :
            return "rbrace";
        case semicolon_ :
            return "semicolon";
        case colon_ :
            return "colon";
        case comma_ :
            return "comma";
        case inplace_ :
            return "inplace";
        case filename_ :
            return "filename";
        case const_ :
            return "const";
        case newline_ :
            return "newline";
        /*case epsilon1_:
            return "epsilon1_";
        case epsilon2_:
            return "epsilon2_";
        */default:
            return "junk";
    }
}

void ParseTable::checkInputTokens(const std::vector<Token> &input) {
    auto root = new AST::AbstractSyntaxTree<Token*>(new Token(root_, ""), 0);
    std::stack<AST::AbstractSyntaxTree<Token*>*> current;
    current.push(root);
    int line_no = 0;
    Token cache(root_, "");
    //TODO Create Abstract syntax tree
    std::vector<std::string> contents{table.begin()->first};
    std::vector<Token> remainingInput = input;
    while(contents.back() != "accept"){
        if(remainingInput.empty()){
            if(table[contents.back()]["EOS"].empty()){
                std::cout<<"error1"<<std::endl;
                //TODO error detection
                return;
            }else {
                if(!computeOperation(contents,remainingInput, table[contents.back()]["EOS"])){
                    std::cout<<"error3"<<std::endl;
                    //TODO error detection
                    return;
                }
            }
        }else {
            Token token = remainingInput.front();
            switch (token.type) {
                case newline_:
                    ++line_no;
                    remainingInput.erase(remainingInput.begin());
                    continue;

                case rparen_:
                case rbrace_:
                    //current.top()->appendChild(new AST::AbstractSyntaxTree<Token *>(new Token(end_, ""), line_no));
                    current.pop();
                    goto exit_check;
                case semicolon_:
                    if (current.top()->getData()->type == incr_ || current.top()->getData()->type == decr_ || current.top()->getData()->type == operator_)
                        current.pop();
                    goto exit_check;

                case identifier_:
                    cache = token;
                    goto exit_check;

                case epsilon1_:
                case epsilon2_:
                    goto exit_check;

                default:
                    break;
            }
            {
                auto new_node = new AST::AbstractSyntaxTree<Token *>(new Token(token), line_no);
                current.top()->appendChild(new_node);

                switch (token.type) {
                    case while_:
                    case incr_:
                    case decr_:
                    case filename_:
                    case print_:
                        current.push(new_node);
                        break;

                    case operator_:
                        if (token.value != "=") break;
                        current.push(new_node);
                        current.top()->appendChild(new AST::AbstractSyntaxTree<Token *>(new Token(cache), line_no));
                        break;

                }
            }
            exit_check:
            if(getTypeString(token.type) == "junk"){
                remainingInput.erase(remainingInput.begin());
                continue;
            }
            if(table[contents.back()][getTypeString(token.type)].empty()){
                std::cout<<"error2"<<std::endl;
                //TODO error detection
                return;
            }else{
                if(!computeOperation(contents, remainingInput, table[contents.back()][getTypeString(token.type)],
                                 getTypeString(token.type))){
                    std::cout<<"error3"<<std::endl;
                    //TODO error detection
                    return;
                }
            }
        }
        for(auto i:contents){
            std::cout<<i;
        }
        std::cout<<std::endl;
    }
}

bool ParseTable::computeOperation(std::vector<std::string> &contents, std::vector<Token> &remainingInput, const std::string &operation, const std::string token) {
    //Shifts
    if(operation.find("shift") != operation.npos){
        contents.push_back(token);
        std::string state = operation.substr(6,state.size()-1);
        contents.push_back(state);
        remainingInput.erase(remainingInput.begin());
    }

    //Productions
    else if(operation.find("->") != operation.npos){
        std::string replaceItems = operation.substr(operation.find("->")+3);
        std::string newVar = operation.substr(0,operation.find("->")-1);

        std::vector<std::string> go_to;
        int pos = 0;
        while(replaceItems.find(" ") != std::string::npos){
            pos = replaceItems.find(" ");
            go_to.push_back(replaceItems.substr(0,pos));
            replaceItems.erase(0,pos+1);
        }
        for(int i = go_to.size()-1; i >= 0; i--){
            if(contents[contents.size()-2-(2*i)] == go_to[go_to.size()-1-i]){
                continue;
            }else {
                return false;
            }
        }
        contents.erase(contents.end()-(2*(go_to.size())), contents.end());
        contents.push_back(newVar);
        contents.push_back(table[contents[contents.size()-2]][newVar]);
    }

    //Accept
    else if(operation == "accept"){
        contents.push_back("accept");
    }
    return true;
}


