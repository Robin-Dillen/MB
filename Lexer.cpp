#include "Lexer.h"

/// private struct
Lexer::Token::Token(TokenType type, const std::string &value) : type(type), value(value) {}

/// class Lexer
Lexer::Lexer(const std::string &characterString) {
    this->tokenize(characterString);
}

void Lexer::tokenize(const std::string &str){

    std::string IdentifierStr;
    std::string NumStr;
    std::string punctStr;

    for(const char Char : str) {

        if (isalpha(Char)) { // identifier: [a-zA-Z][a-zA-Z]*
            /// check if we just stoped reading a number
            if(!NumStr.empty()){
                Tokens.push_back(Token(number_, NumStr));
                NumStr = "";
            }

            IdentifierStr += Char;

            if (IdentifierStr == "incr"){
                Tokens.push_back(Token(incr_, IdentifierStr));
                IdentifierStr = "";
            }
            if (IdentifierStr == "decr"){
                Tokens.push_back(Token(decr_, IdentifierStr));
                IdentifierStr = "";
            }
            if (IdentifierStr == "clear"){
                Tokens.push_back(Token(clear_, IdentifierStr));
                IdentifierStr = "";
            }
            if (IdentifierStr == "while"){
                Tokens.push_back(Token(while_, IdentifierStr));
                IdentifierStr = "";
            }
        }

        else if (isdigit(Char) || Char == '.') {   // Number: [0-9.]+
            if(!IdentifierStr.empty()){
                Tokens.push_back(Token(identifier_, IdentifierStr));
                IdentifierStr = "";
            }

            NumStr += Char;

        }

        else if (ispunct(Char)) {   // punctuation character : [!"#$%&'()*+,-./:;<=>?@[\]^_`{|}~.]
            /// check if we just stoped reading an IdentifierStr
            if(!IdentifierStr.empty()){
                Tokens.push_back(Token(identifier_, IdentifierStr));
                IdentifierStr = "";
            }
                /// check if we just stoped reading a number
            else if(!NumStr.empty()){
                Tokens.push_back(Token(number_, NumStr));
                NumStr = "";
            }

            if(!IdentifierStr.empty()){
                Tokens.push_back(Token(identifier_, IdentifierStr));
                IdentifierStr = "";
            }
            else if(!NumStr.empty()){
                Tokens.push_back(Token(number_, NumStr));
                NumStr = "";
            }
            punctStr += Char;

            if (punctStr == "<"){
                Tokens.push_back(Token(operator_, punctStr));
                punctStr = "";
            }
            else if (punctStr == ">"){
                Tokens.push_back(Token(operator_, punctStr));
                punctStr = "";
            }
            else if (punctStr == "="){
                Tokens.push_back(Token(operator_, punctStr));
                punctStr = "";
            }
            else if (punctStr == "<="){
                Tokens.push_back(Token(operator_, punctStr));
                punctStr = "";
            }
            else if (punctStr == ">="){
                Tokens.push_back(Token(operator_, punctStr));
                punctStr = "";
            }
            else if (punctStr == "!="){
                Tokens.push_back(Token(operator_, punctStr));
                punctStr = "";
            }
            else if (punctStr == "{"){
                Tokens.push_back(Token(lbrace_, punctStr));
                punctStr = "";
            }
            else if (punctStr == "}"){
                Tokens.push_back(Token(rbrace_, punctStr));
                punctStr = "";
            }
            else if (punctStr == "("){
                Tokens.push_back(Token(lparen_, punctStr));
                punctStr = "";
            }
            else if (punctStr == ")"){
                Tokens.push_back(Token(rparen_, punctStr));
                punctStr = "";
            }
            else if (punctStr == ";"){
                Tokens.push_back(Token(semicolon_, punctStr));
                punctStr = "";
            }
        }
    }
}

void Lexer::printTokens(){
    for(Token token : Tokens) {
        switch (token.type){
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
            case clear_:
                std::cout << "[Clear] \t = ";
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
            default:
                std::cout << "[UnknownType] \t = ";
        }
        std::cout << token.value << std::endl;
    }
}
