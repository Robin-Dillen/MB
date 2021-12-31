
#include "Data.h"

#include <utility>

// ------------------------------------- Root -------------------------------------

bool Root::operator==(node_type type) const {
    return false;
}

node_type Root::getType() const {
    return root_;
}

// ------------------------------------- While -------------------------------------

bool While::operator==(node_type type) const {
    return while_ == type;
}

node_type While::getType() const {
    return while_;
}

// ------------------------------------- EndWhile -------------------------------------

bool EndWhile::operator==(node_type type) const {
    return endwhile_ == type;
}

node_type EndWhile::getType() const {
    return endwhile_;
}

// ------------------------------------- Incr -------------------------------------

bool Incr::operator==(node_type type) const {
    return incr_ == type;
}

node_type Incr::getType() const {
    return incr_;
}

// ------------------------------------- Decr -------------------------------------

bool Decr::operator==(node_type type) const {
    return decr_ == type;
}

node_type Decr::getType() const {
    return decr_;
}

// ------------------------------------- Store -------------------------------------

bool Store::operator==(node_type type) const {
    return store_ == type;
}

node_type Store::getType() const {
    return store_;
}

// ------------------------------------- Print -------------------------------------

bool Print::operator==(node_type type) const {
    return print_ == type;
}

node_type Print::getType() const {
    return print_;
}

// ------------------------------------- Func -------------------------------------

Func::Func(bool inline_) : inline_(inline_){}

bool Func::operator==(node_type type) const {
    return func_ == type;
}

node_type Func::getType() const {
    return func_;
}

bool Func::get() const {
    return inline_;
}

// ------------------------------------- Identifier -------------------------------------

Identifier::Identifier(std::string name) : name(std::move(name)) {}

bool Identifier::operator==(node_type type) const {
    return identifier_ == type;
}

node_type Identifier::getType() const {
    return identifier_;
}

std::string Identifier::get() const{
    return name;
}

// ------------------------------------- Const -------------------------------------

Const::Const(unsigned int value) : value(value) {}

bool Const::operator==(node_type type) const {
    return const_ == type;
}

node_type Const::getType() const {
    return const_;
}

unsigned int Const::get() const {
    return value;
}
