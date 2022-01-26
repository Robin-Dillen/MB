
#include "Data.h"

#include <utility>

// ------------------------------------- Root -------------------------------------

bool Root::operator==(TokenType type) const {
    return false;
}

TokenType Root::getType() const {
    return root_;
}

// ------------------------------------- While -------------------------------------

bool While::operator==(TokenType type) const {
    return while_ == type;
}

TokenType While::getType() const {
    return while_;
}

// ------------------------------------- Incr -------------------------------------

bool Incr::operator==(TokenType type) const {
    return incr_ == type;
}

TokenType Incr::getType() const {
    return incr_;
}

// ------------------------------------- Decr -------------------------------------

bool Decr::operator==(TokenType type) const {
    return decr_ == type;
}

TokenType Decr::getType() const {
    return decr_;
}

// ------------------------------------- Store -------------------------------------

bool Store::operator==(TokenType type) const {
    return operator_ == type;
}

TokenType Store::getType() const {
    return operator_;
}

// ------------------------------------- Print -------------------------------------

bool Print::operator==(TokenType type) const {
    return print_ == type;
}

TokenType Print::getType() const {
    return print_;
}

// ------------------------------------- Func -------------------------------------

Func::Func(bool inline_) : inline_(inline_) {}

bool Func::operator==(TokenType type) const {
    return func_ == type;
}

TokenType Func::getType() const {
    return func_;
}

bool Func::get() const {
    return inline_;
}

// ------------------------------------- Identifier -------------------------------------

Identifier::Identifier(std::string name) : name(std::move(name)) {}

bool Identifier::operator==(TokenType type) const {
    return identifier_ == type;
}

TokenType Identifier::getType() const {
    return identifier_;
}

std::string Identifier::get() const {
    return name;
}

// ------------------------------------- Const -------------------------------------

Const::Const(unsigned int value) : value(value) {}

bool Const::operator==(TokenType type) const {
    return const_ == type;
}

TokenType Const::getType() const {
    return const_;
}

unsigned int Const::get() const {
    return value;
}

// ------------------------------------- End -------------------------------------

bool End::operator==(TokenType type) const {
    return end_ == type;
}

TokenType End::getType() const {
    return end_;
}
