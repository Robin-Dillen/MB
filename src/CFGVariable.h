//
// Created by niels on 12/10/2021.
//

#ifndef PROGRAMMEEROPDRACHT_VARIABLE_H
#define PROGRAMMEEROPDRACHT_VARIABLE_H

#include <vector>
#include <string>


class Variable {

public:
    Variable(const std::vector<std::vector<std::string>> &productions, const std::string &name);

    const std::string &getName() const;

    void deleteProduction(std::vector<std::string> prod);

    void addProduction(std::vector<std::string> prod);

    const std::vector<std::vector<std::string>> &getProductions() const;

    int countProds();

    void editProduction(int indexProds, int indexProd, std::string change, bool tilend = false, bool replace = true);

    bool hasProduction(std::vector<std::string> a);

private:
    std::vector<std::vector<std::string>> productions;
    std::string name;
};


#endif //PROGRAMMEEROPDRACHT_VARIABLE_H
