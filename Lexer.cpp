#include "Lexer.h"

/// struct
Token::Token(TokenType type, const std::string &value) : type(type), value(value) {}

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

            if(readfunction){
                filenameStr += Char;
            }
            else{
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
            } else if (std::find(filenames.begin(), filenames.end(), IdentifierStr) != filenames.end()){
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
        }
        else{
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
        switch (token.type) {
            case while_:
                std::cout << "[While] \t = ";
                break;
            case incr_:
                std::cout << "[Incr] \t\t = ";
                break;
            case decr_:
                std::cout << "[Decr] \t\t = ";
                break;
            case print_:
                std::cout << "[Print] \t = ";
                break;
            case identifier_:
                std::cout << "[Identifier] \t = ";
                break;
            case number_:
                std::cout << "[Number] \t = ";
                break;
            case operator_:
                std::cout << "[Operator] \t = ";
                break;
            case lparen_:
                std::cout << "[Lparen] \t = ";
                break;
            case rparen_:
                std::cout << "[Rparen] \t = ";
                break;
            case lbrace_:
                std::cout << "[Lbrace] \t = ";
                break;
            case rbrace_:
                std::cout << "[Rbrace] \t = ";
                break;
            case semicolon_:
                std::cout << "[Semicolon] \t = ";
                break;
            case filename_:
                std::cout << "[Filename] \t = ";
                break;
            case inplace_:
                std::cout << "[Inplace] \t = ";
                break;
            case comma_:
                std::cout << "[Comma] \t = ";
                break;
            case colon_:
                std::cout << "[Colon] \t = ";
                break;
            case const_:
                std::cout << "[Const] \t = ";
                break;
            case import_:
                std::cout << "[Import] \t = ";
                break;
            default:
                std::cout << "[UnknownType] \t = ";
        }
        std::cout << token.value << std::endl;
    }
}

const std::vector<Token> &Lexer::getTokens() const {
    return Tokens;
}
