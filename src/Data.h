#include "lib.h"
#include <string>

#ifndef MB_DATA_H
#define MB_DATA_H


class Data {
public:
    virtual bool operator==(TokenType type) const = 0;

    virtual TokenType getType() const = 0;
};

class Root : public Data {
public:
    bool operator==(TokenType type) const override;

    TokenType getType() const override;
};

class While : public Data {
public:
    bool operator==(TokenType type) const override;

    TokenType getType() const override;
};

class Incr : public Data {
public:
    bool operator==(TokenType type) const override;

    TokenType getType() const override;
};

class Decr : public Data {
public:
    bool operator==(TokenType type) const override;

    TokenType getType() const override;
};

class Store : public Data {
public:
    bool operator==(TokenType type) const override;

    TokenType getType() const override;
};

class Print : public Data {
public:
    bool operator==(TokenType type) const override;

    TokenType getType() const override;
};

class Func : public Data {
public:
    explicit Func(bool inline_);

    bool operator==(TokenType type) const override;

    TokenType getType() const override;

    bool get() const;

private:
    bool inline_;
};

class Identifier : public Data {
public:
    explicit Identifier(std::string name);

    bool operator==(TokenType type) const override;

    TokenType getType() const override;

    std::string get() const;

private:
    std::string name;
};


class Const : public Data {
public:
    explicit Const(unsigned int value);

    bool operator==(TokenType type) const override;

    TokenType getType() const override;

    unsigned int get() const;

private:
    unsigned int value;
};

class End : public Data {
public:
    bool operator==(TokenType type) const override;

    TokenType getType() const override;
};


#endif //MB_DATA_H
