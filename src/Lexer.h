#ifndef MB_LEXER_H
#define MB_LEXER_H

#include <string>
#include <vector>
#include "lib.h"

class Lexer {
public:
    explicit Lexer(const std::string &characterString);

    void tokenize(const std::string &str);

    void printTokens();

    void printTokenString();

    const std::vector<Token> &getTokens() const;

private:

    std::string getTokenName(const Token &token) const;

    std::vector<Token> Tokens;
};


#endif //MB_LEXER_H
