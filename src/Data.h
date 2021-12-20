#include "lib.h"
#include <string>

#ifndef MB_DATA_H
#define MB_DATA_H


class Data {
public:
    virtual bool operator==(node_type type) const = 0;

    virtual node_type getType() const = 0;

    template<typename type>
    type get() const{
        return {};
    }

private:
    std::string name;
};

class While: public Data{
public:
    bool operator==(node_type type) const override;

    node_type getType() const override;
};

class Incr: public Data{
public:
    bool operator==(node_type type) const override;

    node_type getType() const override;
};

class Decr: public Data{
public:
    bool operator==(node_type type) const override;

    node_type getType() const override;
};

class Store: public Data{
public:
    bool operator==(node_type type) const override;

    node_type getType() const override;
};

class Print: public Data{
public:
    bool operator==(node_type type) const override;

    node_type getType() const override;
};

class Func: public Data{
public:
    bool operator==(node_type type) const override;

    node_type getType() const override;

    bool get() const{
        return inline_;
    }

private:
    bool inline_;
};

class Identifier: public Data{
public:
    bool operator==(node_type type) const override;

    node_type getType() const override;

    std::string get() const{
        return name;
    }

private:
    std::string name;
};


class Const: public Data{
public:
    bool operator==(node_type type) const override;

    node_type getType() const override;

    unsigned int get() const{
        return value;
    }

private:
    unsigned int value;
};


#endif //MB_DATA_H
