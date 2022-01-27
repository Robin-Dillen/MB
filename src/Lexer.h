#ifndef MB_LEXER_H
#define MB_LEXER_H

#include "lib.h"

#include <vector>
#include <string>

class Lexer {
public:
    Lexer(const std::string &characterString);

    void tokenize(const std::string &str);

    void printTokens();

    void printTokenString();

    const std::vector<Token> &getTokens();

private:

    std::string getTokenName(const Token &token) const;

    std::vector<Token> Tokens;

    void swapLastEl(std::vector<Token> &vector1);

    void addEpsilon();
};


#endif //MB_LEXER_H
