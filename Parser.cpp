
#include "Parser.h"

Parser::Parser() {
    cfg = new CFG("CFG's/CFG_test.json");
    cfg->print();
    cfg->add_augmented_productions(locSymbole);
    cfg->print();
    canonicalCollection();
}

void Parser::canonicalCollection() {
    //state I0:
    Variable* start = cfg->getStartSymbole();
    //cfg->getClosure(start->getProductions()[0][0]);
    //cfg.start->getProductions();
}

