
#include "Data.h"

bool While::operator==(node_type type) const {
    return while_ == type;
}

node_type While::getType() const {
    return while_;
}

bool Incr::operator==(node_type type) const {
    return incr_ == type;
}

node_type Incr::getType() const {
    return incr_;
}

bool Decr::operator==(node_type type) const {
    return decr_ == type;
}

node_type Decr::getType() const {
    return decr_;
}

bool Store::operator==(node_type type) const {
    return store_ == type;
}

node_type Store::getType() const {
    return store_;
}

bool Print::operator==(node_type type) const {
    return print_ == type;
}

node_type Print::getType() const {
    return print_;
}

bool Func::operator==(node_type type) const {
    return func_ == type;
}

node_type Func::getType() const {
    return func_;
}

bool Identifier::operator==(node_type type) const {
    return identifier_ == type;
}

node_type Identifier::getType() const {
    return identifier_;
}


bool Const::operator==(node_type type) const {
    return const_ == type;
}

node_type Const::getType() const {
    return const_;
}
