//
// Created by Khemin on 11-12-2021.
//
#include "ParseTable.h"
#include "lib.h"

#include <fstream>
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

AST::AbstractSyntaxTree<Token*>* ParseTable::checkInputTokens(const std::vector<Token> &input) {
    auto root = new AST::AbstractSyntaxTree<Token*>(new Token(root_, ""), 0);
    std::stack<AST::AbstractSyntaxTree<Token*>*> current;
    current.push(root);
    int line_no = 1;
    Token cache(root_, "");
    std::vector<std::string> contents{table.begin()->first};
    std::vector<Token> remainingInput = input;
    while(contents.back() != "accept"){
        if(remainingInput.empty()){
            if(table[contents.back()]["EOS"].empty()){
                std::cout << "Missing token(s) on line:" << line_no - 1 <<"." << std::endl;
                return nullptr;
            }else {
                if(!computeOperation(contents,remainingInput, table[contents.back()]["EOS"])){
                    std::cout << "Missing token(s) on line:" << line_no - 1 <<"." << std::endl;
                    return nullptr;
                }
            }
        }else {
            Token token = remainingInput.front();
            switch (token.type) {
                case newline_:
                    ++line_no;
                    remainingInput.erase(remainingInput.begin());
                    continue;

                case lparen_:
                    goto exit_check;

                case lbrace_:
                case rparen_:
                    current.pop();
                    goto exit_check;

                case rbrace_:
                    current.top()->appendChild(new AST::AbstractSyntaxTree<Token *>(new Token(endwhile_, "end"), line_no));
                    current.pop();
                    goto exit_check;
                case semicolon_:
                    if (current.top()->getData()->type == func_)
                        current.top()->appendChild(new AST::AbstractSyntaxTree<Token *>(new Token(endfunc_, "end"), line_no));
                    if (current.top()->getData()->type == incr_ || current.top()->getData()->type == decr_ || current.top()->getData()->type == operator_ || current.top()->getData()->type == func_)
                        current.pop();
                    goto exit_check;

                case inplace_:
                case import_:
                case identifier_:
                    if (current.top()->getData()->type == func_ || current.top()->getData()->type == print_ || remainingInput[1].type != operator_) break;
                    cache = token;
                    goto exit_check;

                case epsilon1_:
                case epsilon2_:
                    goto exit_check;

                default:
                    break;
            }
            {
                AST::AbstractSyntaxTree<Token *>* new_node;
                if (token.type == filename_){
                    if (cache.type == import_) goto exit_check;
                    new_node = new AST::AbstractSyntaxTree<Token *>(new Token(func_, cache.type == const_ ? cache.value: "0"), line_no);
                    current.top()->appendChild(new_node);
                    new_node->appendChild(new AST::AbstractSyntaxTree<Token *>(new Token(identifier_, token.value), line_no));
                }
                else {
                    new_node = new AST::AbstractSyntaxTree<Token *>(new Token(token), line_no);
                    current.top()->appendChild(new_node);
                }

                switch (token.type) {
                    case while_:
                    case incr_:
                    case decr_:
                    case filename_:
                    case print_:
                        current.push(new_node);
                        break;

                    case operator_:
                        current.push(new_node);
                        current.top()->appendChild(new AST::AbstractSyntaxTree<Token *>(new Token(cache), line_no));
                        break;

                }
            }
            exit_check:
            int old_size = remainingInput.size();
            while (remainingInput.size() == old_size) {
                if (table[contents.back()][getTypeString(token.type)].empty()) {
                    std::cout << "Missing token on line:" << line_no - 1 << " Got: " << getTypeString(token.type) << std::endl;
                    return nullptr;
                } else {
                    if (!computeOperation(contents, remainingInput, table[contents.back()][getTypeString(token.type)],
                                          getTypeString(token.type))) {
                        std::cout << "Line "<<line_no-1<<": Token of type "<<token.type<<" with value "<<token.value<<" is not valid in this sequence for the language." << std::endl;
                        return nullptr;
                    }
                }
            }
        }
    }
    return root;
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


