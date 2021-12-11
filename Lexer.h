#ifndef MB_LEXER_H
#define MB_LEXER_H

#include <iostream>
#include <vector>

enum TokenType {
    while_, incr_, decr_, print_, clear_, identifier_, number_, operator_, lparen_, rparen_, lbrace_, rbrace_, semicolon_
};



class Lexer {
public:
    Lexer(const std::string &characterString);

    void tokenize(const std::string &str);

    void printTokens();

private:

    struct Token{
        Token(TokenType type, const std::string &value);

        TokenType type;
        std::string value;
    };

    std::vector<Token> Tokens;
};


#endif //MB_LEXER_H
