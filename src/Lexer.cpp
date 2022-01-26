
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

    bool readfunction = false;

    for (const char Char: str) {

        if (isalpha(Char)) { // identifier: [a-zA-Z][a-zA-Z]*
            /// check if we just stopped reading a number
            if (!NumStr.empty()) {
                Tokens.push_back(Token(number_, NumStr));
                NumStr = "";
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
            } else if (IdentifierStr == "const") {
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

            NumStr += Char;

        } else if (ispunct(Char)) {   // punctuation character : [!"#$%&'()*+,-./:;<=>?@[\]^_`{|}~.]
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

            punctStr += Char;

            if (punctStr == "<") {
                Tokens.push_back(Token(operator_, punctStr));
                punctStr = "";
            } else if (punctStr == ">") {
                Tokens.push_back(Token(operator_, punctStr));
                punctStr = "";
            } else if (punctStr == "=") {
                Tokens.push_back(Token(operator_, punctStr));
                punctStr = "";
            } else if (punctStr == "<=") {
                Tokens.push_back(Token(operator_, punctStr));
                punctStr = "";
            } else if (punctStr == ">=") {
                Tokens.push_back(Token(operator_, punctStr));
                punctStr = "";
            } else if (punctStr == "!=") {
                Tokens.push_back(Token(operator_, punctStr));
                punctStr = "";
            } else if (punctStr == "{") {
                Tokens.push_back(Token(lbrace_, punctStr));
                punctStr = "";
            } else if (punctStr == "}") {
                Tokens.push_back(Token(rbrace_, punctStr));
                punctStr = "";
            } else if (punctStr == "(") {
                Tokens.push_back(Token(lparen_, punctStr));
                punctStr = "";
            } else if (punctStr == ")") {
                Tokens.push_back(Token(rparen_, punctStr));
                punctStr = "";
            } else if (punctStr == ";") {
                Tokens.push_back(Token(semicolon_, punctStr));
                punctStr = "";
            } else if (punctStr == ",") {
                Tokens.push_back(Token(comma_, punctStr));
                punctStr = "";
            } else if (punctStr == ":") {
                Tokens.push_back(Token(colon_, punctStr));
                punctStr = "";
            }
        } else {
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
        }
    }
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

void Lexer::printTokenString() {
    for (Token token: Tokens) {
        std::cout << getTokenName(token) << "";
    }
    std::cout << std::endl;
}

const std::vector<Token> &Lexer::getTokens() const {
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
        default:
            return "unknownType";
    }
}