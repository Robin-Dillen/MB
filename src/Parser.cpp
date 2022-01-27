
#include "Parser.h"

#include <algorithm>

using namespace std;

Parser::Parser(CFG *c) {
    cfg = c;
    cfg->add_augmented_productions(locSymbole);
    canonicalCollection();
}

void Parser::canonicalCollection() {
    //state I0:
    Variable *start = cfg->getStartSymbole();
    vector<Variable *> variables = {};
    cfg->getClosure(start->getName(), variables);
    map<string, vector<vector<string>>> productions = varsToProds(variables);
    DFAState *stateZero = new DFAState("0", productions, false);
    UpperRoot = stateZero;
    dfaStates.push_back(stateZero);
    create_canonical_states(stateZero);

}

map<string, vector<vector<string>>> Parser::varsToProds(const vector<Variable *> &variables) {
    map<string, vector<vector<string>>> prods;
    for (auto it: variables) {
        prods[it->getName()] = it->getProductions();
    }
    return prods;
}

void Parser::create_canonical_states(DFAState *rootState) {
    for (auto mapItem: rootState->getContent()) {
        for (auto production: mapItem.second) {
            int i = checkSymLoc(production);
            if (i != production.size() - 1) {
                map<string, vector<vector<string>>> newStateProductions = goTo(rootState, production[i + 1]);
                moveLocSym(newStateProductions);
                getClosure(newStateProductions);
                DFAState *existingState = stateExists(newStateProductions);
                if (existingState != nullptr) {
                    rootState->addTransition(production[i + 1], existingState);
                } else {

                    string name = to_string(dfaStates.size());
                    DFAState *state = new DFAState(name, newStateProductions, false);
                    dfaStates.push_back(state);
                    rootState->addTransition(production[i + 1], state);
                    create_canonical_states(state);


                }

            }
        }
    }
}

void Parser::moveLocSym(map<string, vector<vector<string>>> &prods) {
    for (auto mapItem = prods.begin(); mapItem != prods.end(); mapItem++) {
        for (auto j = 0; j < mapItem->second.size(); j++) {
            int i = checkSymLoc(mapItem->second[j]);
            swap(mapItem->second[j][i + 1], mapItem->second[j][i]);
        }
    }
}

void Parser::getClosure(map<string, vector<vector<string>>> &prods) {
    map<string, vector<vector<string>>> closure;
    for (auto mapItem = prods.begin(); mapItem != prods.end(); mapItem++) {
        for (auto i = 0; i < mapItem->second.size(); i++) {
            int j = checkSymLoc(mapItem->second[i]);
            if (mapItem->second[i].size() - 1 > j) {
                getClosure(mapItem->second[i][j + 1], closure);
            }
        }
    }
    for (auto mapItem = closure.begin(); mapItem != closure.end(); mapItem++) {
        if (prods.find(mapItem->first) == prods.end()) {
            prods[mapItem->first] = mapItem->second;
        } else {
            for (auto production = mapItem->second.begin(); production != mapItem->second.end(); production++) {
                if (!count(prods[mapItem->first].begin(), prods[mapItem->first].end(), *production)) {
                    prods[mapItem->first].push_back(*production);
                }
            }
        }
    }
}

void Parser::getClosure(string item, map<string, vector<vector<string>>> &closure) {
    map<string, vector<vector<string>>> root = UpperRoot->getContent();
    if (root.find(item) == root.end()) return;
    vector<vector<string>> prods = root[item];
    closure[item] = prods;
    for (int j = 0; j < prods.size(); j++) {
        int i = checkSymLoc(prods[j]);
        if(i < prods[j].size()-1){
            if (closure.find(prods[j][i+1]) != closure.end()) continue;
            getClosure(prods[j][i+1], closure);
        }
    }
}

map<string, vector<vector<string>>> Parser::goTo(const DFAState *rootState, string s) {
    map<string, vector<vector<string>>> newStateProductions;
    for (auto mapItem: rootState->getContent()) {
        for (auto production: mapItem.second) {
            int i = checkSymLoc(production);
            if (i != production.size() - 1) {
                if (production[i + 1] == s) {
                    newStateProductions[mapItem.first].push_back(production);
                }
            }
        }
    }
    return newStateProductions;
}

int Parser::checkSymLoc(const vector<string> &production) {
    for (int i = 0; i < production.size(); i++) {
        if (production[i] == locSymbole) return i;
    }
    return -1;
}

DFAState *Parser::stateExists(const map<string, vector<vector<string>>> &prods) {
    for (auto it: dfaStates) {
        if (it->getContent() == prods) {
            return it;
        }
    }
    return nullptr;
}

void Parser::printState(map<string, vector<vector<string>>> prods) {
    for (auto it: prods) {
        vector<vector<string>> productions = it.second;
        for (int j = 0; j < productions.size(); j++) {
            string production;
            for (int k = 0; k < productions[j].size(); k++) {
                production += productions[j][k];
                if (k != productions[j].size() - 1) {
                    production += " ";
                }
            }
            cout << "  " << it.first << " -> `" << production << "`" << endl;
        }
    }
    cout << endl;
}

ParseTable Parser::getParseTable() {
    DFA dfa = DFA(dfaStates, cfg->getTerminals(), cfg->getVariables());

    return ParseTable(dfa);
}