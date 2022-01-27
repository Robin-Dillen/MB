
#include "Lexer.h"
#include <iostream>
#include <algorithm>
#include <cmath>


/// class Lexer
Lexer::Lexer(const std::string &characterString) {
    this->tokenize(characterString);
}

void Lexer::tokenize(const std::string &str) {

    std::vector<std::string> filenames;

    std::string IdentifierStr;
    std::string filenameStr;
    std::string NumStr;
    std::string punctStr;
    std::string opertatorStr;

    bool readfunction = false;
    int count = 1;

    for (const char Char: str) {

        // check for new line
        if (Char == '\n') {
            Tokens.push_back(Token(newline_, std::to_string(count)));
            count += 1;
        }

        if (isalpha(Char) || (isdigit(Char) && !IdentifierStr.empty())) { // identifier: [a-zA-Z][a-zA-Z]*
            /// check if we just stopped reading a number
            if (!NumStr.empty()) {
                Tokens.push_back(Token(number_, NumStr));
                NumStr = "";
            }
            /// check if we just stopped reading an operator
            if (!opertatorStr.empty()) {
                Tokens.push_back(Token(operator_, opertatorStr));
                opertatorStr = "";
            }

            if (readfunction) {
                filenameStr += Char;
            } else {
                IdentifierStr += Char;
            }

            if (IdentifierStr == "incr") {
                Tokens.push_back(Token(incr_, IdentifierStr));
                IdentifierStr = "";
            } else if (IdentifierStr == "decr") {
                Tokens.push_back(Token(decr_, IdentifierStr));
                IdentifierStr = "";
            } else if (IdentifierStr == "while") {
                Tokens.push_back(Token(while_, IdentifierStr));
                IdentifierStr = "";
            } else if (IdentifierStr == "print") {
                Tokens.push_back(Token(print_, IdentifierStr));
                IdentifierStr = "";
            }else if (IdentifierStr == "const") {
                Tokens.push_back(Token(const_, IdentifierStr));
                IdentifierStr = "";
            } else if (IdentifierStr == "inplace") {
                Tokens.push_back(Token(inplace_, IdentifierStr));
                IdentifierStr = "";
            } else if (IdentifierStr == "import") {
                Tokens.push_back(Token(import_, IdentifierStr));
                IdentifierStr = "";
                readfunction = true;
            } else if (std::find(filenames.begin(), filenames.end(), IdentifierStr) != filenames.end()) {
                Tokens.push_back(Token(filename_, IdentifierStr));
                IdentifierStr = "";
            }
        } else if (isdigit(Char) || Char == '.') {   // Number: [0-9.]+
            /// check if we just stopped reading an IdentifierStr
            if (!IdentifierStr.empty()) {
                Tokens.push_back(Token(identifier_, IdentifierStr));
                IdentifierStr = "";
            }
            /// check if we just stopped reading a filenameStr
            if (!filenameStr.empty()) {
                Tokens.push_back(Token(filename_, filenameStr));
                filenames.push_back(filenameStr);
                filenameStr = "";
                readfunction = false;
            }
            /// check if we just stopped reading an operator
            if (!opertatorStr.empty()) {
                Tokens.push_back(Token(operator_, opertatorStr));
                opertatorStr = "";
            }

            NumStr += Char;

        } else if (ispunct(Char)) {   // punctuation character : [!"#$%&'()*+,-./:;<=>?@[\]^_`{|}~.]
            bool punctfound = false;

            /// check if we just stopped reading an IdentifierStr
            if (!IdentifierStr.empty()) {
                Tokens.push_back(Token(identifier_, IdentifierStr));
                IdentifierStr = "";
            }
            /// check if we just stopped reading a number
            if (!NumStr.empty()) {
                Tokens.push_back(Token(number_, NumStr));
                NumStr = "";
            }
            /// check if we just stopped reading a filenameStr
            if (!filenameStr.empty()) {
                Tokens.push_back(Token(filename_, filenameStr));
                filenames.push_back(filenameStr);
                filenameStr = "";
                readfunction = false;
            }


            punctStr = Char;

            if (punctStr == "{") {
                Tokens.push_back(Token(lbrace_, punctStr));
                punctStr = "";
                punctfound = true;
            } else if (punctStr == "}") {
                Tokens.push_back(Token(epsilon2_, "#"));
                Tokens.push_back(Token(rbrace_, punctStr));
                punctStr = "";
                punctfound = true;
            } else if (punctStr == "(") {
                Tokens.push_back(Token(lparen_, punctStr));
                punctStr = "";
                punctfound = true;
            } else if (punctStr == ")") {
                Tokens.push_back(Token(rparen_, punctStr));
                punctStr = "";
                punctfound = true;
            } else if (punctStr == ";") {
                Tokens.push_back(Token(semicolon_, punctStr));
                punctStr = "";
                punctfound = true;
            } else if (punctStr == ",") {
                Tokens.push_back(Token(comma_, punctStr));
                punctStr = "";
                punctfound = true;
            } else if (punctStr == ":") {
                Tokens.push_back(Token(colon_, punctStr));
                punctStr = "";
                punctfound = true;
            }else{
                opertatorStr += Char;
            }
            if(punctfound && !opertatorStr.empty()){
                Tokens.push_back(Token(operator_, opertatorStr));
                opertatorStr = "";
                swapLastEl(Tokens);
            }
        } else{
            /// check if we just stopped reading an IdentifierStr
            if (!IdentifierStr.empty()) {
                Tokens.push_back(Token(identifier_, IdentifierStr));
                IdentifierStr = "";
            }
            /// check if we just stopped reading an operator
            if (!opertatorStr.empty()) {
                Tokens.push_back(Token(operator_, opertatorStr));
                opertatorStr = "";
            }
            /// check if we just stopped reading a number
            if (!NumStr.empty()) {
                Tokens.push_back(Token(number_, NumStr));
                NumStr = "";
            }
            /// check if we just stopped reading a filenameStr
            if (!filenameStr.empty()) {
                Tokens.push_back(Token(filename_, filenameStr));
                filenames.push_back(filenameStr);
                filenameStr = "";
                readfunction = false;
            }
        }
    }
    /// check if we just stopped reading an IdentifierStr
    if (!IdentifierStr.empty()) {
        Tokens.push_back(Token(identifier_, IdentifierStr));
    }
    /// check if we just stopped reading an operator
    if (!opertatorStr.empty()) {
        Tokens.push_back(Token(operator_, opertatorStr));
    }
    /// check if we just stopped reading a number
    if (!NumStr.empty()) {
        Tokens.push_back(Token(number_, NumStr));
    }
    /// check if we just stopped reading a filenameStr
    if (!filenameStr.empty()) {
        Tokens.push_back(Token(filename_, filenameStr));
        filenames.push_back(filenameStr);
    }
    this->addEpsilon();
}

void Lexer::printTokens() {

    for (Token token: Tokens) {
        std::cout << "[" << getTokenName(token) << "]";
        for (int i = 0; i <= 1 - floor(((double) getTokenName(token).size() + 2.0) / 8.0); i++) {
            std::cout << "\t";
        }
        std::cout << "= " << token.value << std::endl;
    }
}

void Lexer::printTokenString(){

    for (Token token: Tokens) {
        if(getTokenName(token) == "newline"){
            std::cout << std::endl;
        }
        else{
            std::cout << getTokenName(token) << " ";
        }
    }
    std::cout << std::endl;
}
void Lexer::addEpsilon(){
    int imprt_counter = 0;
    for (Token token: Tokens) {
        if(token.type == import_){
            imprt_counter += 1;
        }
    }
    if(imprt_counter != 0){
        int index = 0;
        bool newline = false;
        int imprt_counter_ = 0;
        for (Token token: Tokens) {
            if(token.type == import_){
                imprt_counter_ += 1;
            }
            if(imprt_counter == imprt_counter_){
                newline = true;
            }
            if(newline && token.type == newline_){
                break;
            }
            index += 1;
            }
        auto itPos = Tokens.begin() + index;
        Tokens.insert(itPos, Token(epsilon1_, "#"));
    }
    else{
        Tokens.insert(Tokens.begin(), Token(epsilon1_, "#"));
    }

    Tokens.push_back(Token(epsilon2_, "#"));
}

const std::vector<Token> &Lexer::getTokens() {

    return Tokens;
}

std::string Lexer::getTokenName(const Token &token) const {
    switch (token.type) {
        case while_:
            return "while";
        case incr_:
            return "incr";
        case decr_:
            return "decr";
        case print_:
            return "print";
        case identifier_:
            return "identifier";
        case number_:
            return "number";
        case operator_:
            return "operator";
        case lparen_:
            return "lparen";
        case rparen_:
            return "rparen";
        case lbrace_:
            return "lbrace";
        case rbrace_:
            return "rbrace";
        case semicolon_:
            return "semicolon";
        case filename_:
            return "filename";
        case inplace_:
            return "inplace";
        case comma_:
            return "comma";
        case colon_:
            return "colon";
        case const_:
            return "const";
        case import_:
            return "import";
        case newline_:
            return "newline";
        case epsilon1_:
            return "epsilon1";
        case epsilon2_:
            return "epsilon2";
        default:
            return "unknownType";
    }
}

void Lexer::swapLastEl(std::vector<Token> &vector1) {
    Token tremp = vector1.back();
    vector1[vector1.size()-1] = vector1[vector1.size()-2];
    vector1[vector1.size()-2] = tremp;
}