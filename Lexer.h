#ifndef MB_LEXER_H
#define MB_LEXER_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

namespace Lexer_elements {
    enum TokenType {
        while_, incr_, decr_, print_, import_, identifier_, number_, operator_, lparen_, rparen_, lbrace_,
        rbrace_, semicolon_, colon_, comma_, inplace_, filename_, const_, newline_
    };
}

struct Token {
    Token(Lexer_elements::TokenType type, const std::string &value);

    Lexer_elements::TokenType type;
    std::string value;
};

class Lexer {
public:
    Lexer(const std::string &characterString);

    void tokenize(const std::string &str);

    void printTokens();

    void printTokenString();

    const std::vector<Token> &getTokens() const;

private:

    std::string getTokenName(const Token &token) const;

    std::vector<Token> Tokens;
};


#endif //MB_LEXER_H
