
#include "CFG.h"


CFG::CFG(const string &inputfile) {

    readfile(inputfile);
}

void CFG::readfile(const string &inputfile){
    ifstream input(inputfile);
    json j;
    input >> j;

    /// We gaan eerst alle componenten van de CFG bepalen de quadruple: {V,T,P,S}

    // Om te sorteren op alfabetische volgorde maak ik tijdelijk een set aan
    set<string> V_temp;
    for (int i = 0; i < j["Variables"].size(); i++) {
        string name = j["Variables"][i];
        V_temp.insert(name);
    }
    for (std::set<string>::iterator it=V_temp.begin(); it!=V_temp.end(); ++it){
        V.push_back(*it);
    }

    set<string> T_temp;
    // Om te sorteren op alfabetische volgorde maak ik tijdelijk een set aan
    for (int i = 0; i < j["Terminals"].size(); i++) {
        string name = j["Terminals"][i];
        T_temp.insert(name);
    }
    for (std::set<string>::iterator it=T_temp.begin(); it!=T_temp.end(); ++it){
        T.push_back(*it);
    }

    for (int i = 0; i < j["Productions"].size(); i++) {
        string name = j["Productions"][i]["head"];
        vector<vector<string>> rules;
        if(!production_rules.count(name)){
            production_rules[name] = rules;
        }
        vector<string> rules_ = j["Productions"][i]["body"];
        if(rules_.empty()){
            rules_ = {""};
            production_rules[name].insert(production_rules[name].begin(), rules_);
        }
        else{
            production_rules[name].push_back(rules_);
        }
    }

    startsymbol = j["Start"];
}



// gives a visual representations of the CFG
void CFG::print(){
    cout << "V = {";
    if (!V.empty()){
        for(int i = 0; i < V.size()-1; i++) {
            cout << V[i] << ", ";
        }
        cout << V[V.size()-1];
    }
    cout << "}" << endl;

    cout << "T = {";
    if (!T.empty()) {
        for (int i = 0; i < T.size() - 1; i++) {
            cout << T[i] << ", ";
        }
        cout << T[T.size() - 1];
    }
    cout << "}" << endl;

    cout << "P = {" << endl;
    // we sorteren de keys
    set<string> keys;
    for (map<string, vector<vector<string>>>::iterator it = production_rules.begin(); it != production_rules.end(); it++){
        keys.insert((*it).first);
    }
    for (string key : keys){
        for(int i = 0; i < production_rules[key].size(); i++) {
            // we sorteren de lijst met productions
            sort(production_rules[key].begin(), production_rules[key].end());

            cout << "  " << key << " -> `";
            for(int j = 0; j < production_rules[key][i].size()-1; j++) {
                cout << production_rules[key][i][j] << " ";
            }
            cout << production_rules[key][i][production_rules[key][i].size()-1] << "`";
            cout << endl;
        }
    }
    cout << "}" << endl;
    cout << "S = " << startsymbol << endl;
}

CFG::CFG(const vector<string> &v, const vector<string> &t, const string &startsymbol,
         const map<string, vector<vector<string>>> &productionRules) : V(v), T(t), startsymbol(startsymbol),
                                                                       production_rules(productionRules) {

}

void CFG::add_augmented_productions() {
    /// we add a new startsymbol
    string new_startsymbool = startsymbol + "'";
    vector<vector<string>> rules;
    // making an augmented production by adding •
    vector<string> rule = {"•", startsymbol};
    rules.push_back(rule);
    production_rules[new_startsymbool] = rules;
    startsymbol = new_startsymbool;

    // making an augmented production by adding • to all the productions
    for (map<string, vector<vector<string>>>::iterator it = production_rules.begin();
         it != production_rules.end(); it++) {
        for (int i = 0; i < production_rules[(*it).first].size(); i++) {
            // uitzondering voor epsilon productions
            if (production_rules[(*it).first][i][0] == "") {
                cout << "bij deze key: " << (*it).first << endl;
                production_rules[(*it).first][i] = {"•"};
            } else {
                production_rules[(*it).first][i].insert(production_rules[(*it).first][i].begin(), "•");
            }
        }
    }
}

/// ---------------TO-CNF--------------------
/***
 * @param production_rules (production rules)
 * @param V (nonterminals)
 * @return vector of all nullable productions
 */

vector<string> get_nullables(map<string, vector<vector<string>>> production_rules,vector<string> V){
    vector<string> nullables;
    vector<string> temp_nullables = {""};
    bool loop = true;
    // we gaan eerst kijken naar welke nonterminals een epsilon transitie hebben (daarom assigneren we dit aan temp_nullables)
    // production rules is van de vorm vb: S -> [[""], ["a", "A", "a"], ["b", "B", "b"]]    ==   (S -> epsilon | aAa | bBb)
    while (loop){
        vector<string> nonterminals;
        for (map<string, vector<vector<string>>>::iterator it = production_rules.begin(); it != production_rules.end(); it++) {
            for (int i = 0; i < production_rules[(*it).first].size(); i++) {
                for (int j = 0; j < production_rules[(*it).first][i].size(); j++) {
                    if (find(temp_nullables.begin(), temp_nullables.end(), production_rules[(*it).first][i][j]) != temp_nullables.end()){
                        // we voegen deze toe aan de nonterminals
                        nonterminals.push_back((*it).first);
                    }
                }
            }
        }
        // als de temp_nullables en de nonterminals gelijk zijn is er deze iteratie niets verandert en hebben we alle Nullables
        if(temp_nullables == nonterminals){
            loop = false;
        }
        temp_nullables = nonterminals;
        nullables.insert(nullables.end(), nonterminals.begin(), nonterminals.end());
    }
    // we halen de nonterminals die er dubbel in zitten er uit
    sort( nullables.begin(), nullables.end() );
    nullables.erase( unique( nullables.begin(), nullables.end() ), nullables.end() );
    return nullables;
}

/***
 * @param production_rules (production rules)
 * @return amount of productions
 */
int count_productions(map<string, vector<vector<string>>> production_rules){
    // we tellen het aantal producties
    int amount_of_productions = 0;
    for (map<string, vector<vector<string>>>::iterator it = production_rules.begin(); it != production_rules.end(); it++) {
        for (int i = 0; i < production_rules[(*it).first].size(); i++) {
            amount_of_productions += 1;
        }
    }
    return amount_of_productions;
}

/// helperfunctie van get_all_combinations_off
void get_all_combinations_off_(vector<string> arr, vector<string> data, int start, int end, int index, int r, vector<vector<string>> &combination){
    if (index == r){
        vector<string> element;
        for (int j = 0; j < r; j++){
            element.push_back(data[j]);
        }
        combination.push_back(element);
        return;
    }

    for (int i = start; i < end && end - i + 1 >= r - index; i++){
        data[index] = arr[i];
        get_all_combinations_off_(arr, data, i+1, end, index+1, r, combination);
    }
}

// inspiratie voor deze functie https://www.geeksforgeeks.org/print-all-possible-combinations-of-r-elements-in-a-given-array-of-size-n/
/***
 * @param V (vector of strings)
 * @return vector<vector<string>> of all the posible combinations whit the elements in the given vector
 */
vector<vector<string>> get_all_combinations_off(vector<string> v){
    vector<vector<string>> all_combinations;
    for (int i = 1; i < v.size()+1; i++){
        vector<string> temp(v.size(), "");
        vector<vector<string>> combination;
        get_all_combinations_off_(v, temp, 0, v.size(), 0, i, combination);

        all_combinations.insert(all_combinations.end(), combination.begin(), combination.end());
    }

    return all_combinations;
}

/***
 * @param production_rule (one production rule)
 * @param nullables (vector of all nullable productions)
 * @return vector<vector<string>> of all the new transitions
 */
vector<vector<string>> new_transitions(vector<string> production_rule, vector<string> nullables){
    vector<vector<string>> newtransitions;
    vector<string> nullables_in_string;

    // we halen de nodige nullables er uit
    for (int i = 0; i < production_rule.size(); i++) {
        if(find(nullables.begin(), nullables.end(), production_rule[i]) != nullables.end()){
            nullables_in_string.push_back(production_rule[i]);
        }
    }

    // eerst berekenen we het aantal mogenlijkheden we kunnen maken
    // en deze gaan we dan uit onze productie verwijderen
    vector<vector<string>> all_combinations = get_all_combinations_off(nullables_in_string);
    for (int i = 0; i < all_combinations.size(); i++) {
        vector<string> production_rule_copy = production_rule;
        for (int j = 0; j < all_combinations[i].size(); j++) {
            production_rule_copy.erase(remove(production_rule_copy.begin(), production_rule_copy.end(), all_combinations[i][j]), production_rule_copy.end());
        }
        if(!production_rule_copy[0].empty()){
            newtransitions.push_back(production_rule_copy);
        }
    }

    return newtransitions;
}

/// helper function of get_unit_pairs
void get_unit_pairs_(map<string, vector<vector<string>>> production_rules, vector<string> V, string current, vector<string> &to){
    for (int i = 0; i < production_rules[current].size(); i++) {
        if(production_rules[current][i].size() == 1 && find(V.begin(), V.end(), production_rules[current][i][0]) != V.end()){
            to.push_back(production_rules[current][i][0]);
            get_unit_pairs_(production_rules, V, production_rules[current][i][0], to);
        }
    }
}

/***
 * @param production_rules (production_rules)
 * @param V (nonterminals)
 * @return vector<vector<string>> of all the unit pairs
 */
vector<pair<string, string>> get_unit_pairs(map<string, vector<vector<string>>> production_rules, vector<string> V){
    vector<pair<string, string>> pairs;
    // alle gewone pairs maken
    for (string nonterminal : V){
        pairs.push_back(make_pair(nonterminal,nonterminal));
    }
    // we gaan recursief alle unit pairs zoeken met de helper functie get_unit_pairs_
    vector<pair<string, string>> pairs_ = pairs;
    for(pair<string, string> pair : pairs){
        vector<string> goes_to;
        get_unit_pairs_(production_rules, V, pair.second, goes_to);
        for(string el : goes_to){
            pairs_.push_back(make_pair(pair.first, el));
        }
    }

    return pairs_;
}

// helperfunctie van get_reachable_symbols
void get_reachable_symbols_(map<string, vector<vector<string>>> production_rules, string staat, vector<string> nonterminals, vector<string> &reachable){
    for (int i = 0; i < production_rules[staat].size(); i++) {
        for (int j = 0; j < production_rules[staat][i].size(); j++) {
            if (find(nonterminals.begin(), nonterminals.end(), production_rules[staat][i][j]) != nonterminals.end()) {
                if(find(reachable.begin(), reachable.end(), production_rules[staat][i][j]) == reachable.end()){
                    reachable.push_back(production_rules[staat][i][j]);
                    get_reachable_symbols_(production_rules, production_rules[staat][i][j], nonterminals, reachable);
                }
            }
            if(find(reachable.begin(), reachable.end(), production_rules[staat][i][j]) == reachable.end()){
                reachable.push_back(production_rules[staat][i][j]);
            }
        }
    }
}

/***
 * @param production_rules (production_rules)
 * @param start_staat (start staat)
 * @param nonterminals (nonterminals)
 * @return vector<string> of all the reachable symbols
 */
vector<string> get_reachable_symbols(map<string, vector<vector<string>>> production_rules, string start_staat, vector<string> nonterminals){
    vector<string> reachable = {start_staat};
    get_reachable_symbols_(production_rules, start_staat, nonterminals, reachable);
    return reachable;
}

void CFG::toCNF(){

    cout << "Original CFG:" << endl << endl;
    this->print();
    cout << endl << "-------------------------------------" << endl << endl;

    /// Eliminating epsilon productions

    // we berekennen de Nullables :
    vector<string> nullables = get_nullables(production_rules, V);


    int first_amount = count_productions(production_rules);


    // nu moeten we de nieuwe producties maken:

    for (map<string, vector<vector<string>>>::iterator it = production_rules.begin(); it != production_rules.end(); it++) {
        vector<vector<string>> newtransitions;
        for (int i = 0; i < production_rules[(*it).first].size(); i++) {
            // we verwijderen de epsilon producties
            sort(production_rules[(*it).first].begin(), production_rules[(*it).first].end());
            // de productieregels zijn gesorteerd dus "" zit altijd vooraan
            if (production_rules[(*it).first][0][0] == "") {
                production_rules[(*it).first].erase(production_rules[(*it).first].begin());
            }

            if (production_rules[(*it).first][i].size() <= 1){
                continue;
            }
            vector<vector<string>> newtransition = new_transitions(production_rules[(*it).first][i], nullables);
            newtransitions.insert(newtransitions.end(), newtransition.begin(), newtransition.end());
        }
        production_rules[(*it).first].insert(production_rules[(*it).first].end(), newtransitions.begin(), newtransitions.end());
    }

    // we berekenen het nieuwe aantal productieregels

    int second_amount = count_productions(production_rules);

    cout << " >> Eliminating epsilon productions" << endl <<  "  Nullables are {";
    if (!nullables.empty()){
        for (int i = 0; i < nullables.size()-1; i++) {
            cout << nullables[i] << ", ";
        }
        cout << nullables[nullables.size()-1] << "}" << endl;
    }
    else{
        cout << "}" << endl;
    }
    cout << "  Created " << second_amount << " productions, original had " << first_amount << endl << endl;

    this->print();
    cout << endl;

    /// Eliminating unit pairs
    vector<pair<string, string>> unit_pairs = get_unit_pairs(production_rules, V);

    int unit_productions = 0;
    for (map<string, vector<vector<string>>>::iterator it = production_rules.begin(); it != production_rules.end(); it++) {
        for (int i = 0; i < production_rules[(*it).first].size(); i++) {
            if(production_rules[(*it).first][i].size() == 1 && find(V.begin(), V.end(), production_rules[(*it).first][i][0]) != V.end()){
                unit_productions += 1;
            }
        }
    }
    first_amount = count_productions(production_rules);

    // we gaan nu de nieuwe producties opstellen
    map<string, vector<vector<string>>> new_production_rules;
    for (int i = 0; i < unit_pairs.size(); i++) {
        vector<vector<string>> rules;
        if(!new_production_rules.count(unit_pairs[i].first)){
            new_production_rules[unit_pairs[i].first] = rules;
        }
        // we gaan de unit pairs af en voegen telkens de producties van het 2 de symbool van het paar toe aan de nieuwe productieregels
        if(unit_pairs[i].first != unit_pairs[i].second){
            vector<string> replacable = {unit_pairs[i].second};
            new_production_rules[unit_pairs[i].first].erase(remove(new_production_rules[unit_pairs[i].first].begin(), new_production_rules[unit_pairs[i].first].end(), replacable), new_production_rules[unit_pairs[i].first].end());
        }
        new_production_rules[unit_pairs[i].first].insert(new_production_rules[unit_pairs[i].first].end(), production_rules[unit_pairs[i].second].begin(), production_rules[unit_pairs[i].second].end());

        /* het is mogelijk dat er dubbels verschijnen in de production rules;
         * vb: (D, D), (D, A), (D, B), (D, C), (D, E)
         *  A -> `C`        |       C -> `D E`      |   S -> `a A a`
         *  A -> `a`        |       C -> `E`        |   S -> `a a`
         *  B -> `C`        |       D -> `A`        |   S -> `b B b`
         *  B -> `b`        |       D -> `B`        |   S -> `b b`
         *  C -> `C D E`    |       D -> `a b`
         *  C -> `C E`      |
         *
         *  D -> A | B | a b
         *  D -> C | a | B | a b
         *  D -> C D E | C E | D E | a | B | a b
         *  D -> C D E | C E | D E | a | C | b | a b                    // een transistie van B naar C hebben we al gehad
         *  D -> C D E | C E | D E | a | C D E | C E | D E | b | a b    // op deze manier komen er dubbele tranisties (en deze zullen er dus uit gehaald worden
         *  D -> C D E | C E | D E | a | b | a b
         */
        sort( new_production_rules[unit_pairs[i].first].begin(), new_production_rules[unit_pairs[i].first].end() );
        new_production_rules[unit_pairs[i].first].erase( unique( new_production_rules[unit_pairs[i].first].begin(), new_production_rules[unit_pairs[i].first].end() ), new_production_rules[unit_pairs[i].first].end() );
    }
    production_rules = new_production_rules;


    // we berekenen het nieuwe aantal productieregels
    sort(unit_pairs.begin(), unit_pairs.end());
    unit_pairs.erase(unique(unit_pairs.begin(), unit_pairs.end()), unit_pairs.end());
    second_amount = count_productions(production_rules);
    cout << " >> Eliminating unit pairs" << endl;
    cout << "  Found "<< unit_productions << " unit productions" << endl;
    cout << "  Unit pairs: {";
    if (!unit_pairs.empty()) {
        for (int i = 0; i < unit_pairs.size() - 1; i++) {
            cout << "(" << unit_pairs[i].first << ", " << unit_pairs[i].second << ")" << ", ";
        }
        cout << "(" << unit_pairs[unit_pairs.size()-1].first << ", "  << unit_pairs[unit_pairs.size()-1].second << ")}" << endl;
    }
    else{
        cout << "}" << endl;
    }
    cout << "  Created " << second_amount << " new productions, original had " << first_amount << endl << endl;

    this->print();
    cout << endl;

    /// Eliminating useless symbols

    vector<string> Generating_symbols = T;
    vector<string> Reachable_symbols;
    vector<string> Useful_symbols;

    first_amount = count_productions(production_rules);

    // we zoeken eerst de generating symbols

    vector<string> temp_Generating_symbols;
    bool diff = true;
    while (diff){
        temp_Generating_symbols = Generating_symbols;
        for (map<string, vector<vector<string>>>::iterator it = production_rules.begin(); it != production_rules.end(); it++) {
            for (int i = 0; i < production_rules[(*it).first].size(); i++) {
                bool all_terminals = true;
                bool in_generating = true;
                vector<string> nonterminals;
                for (int j = 0; j < production_rules[(*it).first][i].size(); j++) {
                    if (find(T.begin(), T.end(), production_rules[(*it).first][i][j]) == T.end()) {
                        all_terminals = false;
                        nonterminals.push_back(production_rules[(*it).first][i][j]);
                    }
                }

                for (int i = 0; i < nonterminals.size(); i++) {
                    if (std::find(Generating_symbols.begin(), Generating_symbols.end(), nonterminals[i]) == Generating_symbols.end()) {
                        in_generating = false;
                    }
                }
                if((all_terminals or in_generating)and find(Generating_symbols.begin(), Generating_symbols.end(), (*it).first) == Generating_symbols.end()){
                    Generating_symbols.push_back((*it).first);
                }

            }
        }
        if (temp_Generating_symbols == Generating_symbols){
            diff = false;
        }
    }

    sort(Generating_symbols.begin(), Generating_symbols.end());

    // we gaan alle producties verwijderen waarbij een non-Generating_symbols in voorkomt
    for (map<string, vector<vector<string>>>::iterator it = production_rules.begin(); it != production_rules.end(); it++) {
        for (int i = 0; i < production_rules[(*it).first].size(); i++) {
            for (int j = 0; j < production_rules[(*it).first][i].size(); j++) {
                if (find(Generating_symbols.begin(), Generating_symbols.end(), production_rules[(*it).first][i][j]) == Generating_symbols.end()) {
                    production_rules[(*it).first].erase(remove(production_rules[(*it).first].begin(), production_rules[(*it).first].end(), production_rules[(*it).first][i]), production_rules[(*it).first].end());
                    i--;
                    break;
                }
            }
        }
    }

    // we zoeken de reachable symbols
    Reachable_symbols = get_reachable_symbols(production_rules, startsymbol, V);
    sort(Reachable_symbols.begin(), Reachable_symbols.end());

    // we gaan alle producties verwijderen waarbij een non-Reachable_symbols in voorkomt
    for (map<string, vector<vector<string>>>::iterator it = production_rules.begin(); it != production_rules.end(); it++) {
        bool deleted = false;
        for (int i = 0; i < production_rules[(*it).first].size(); i++) {
            for (int j = 0; j < production_rules[(*it).first][i].size(); j++) {
                if (find(Reachable_symbols.begin(), Reachable_symbols.end(), production_rules[(*it).first][i][j]) == Reachable_symbols.end()) {
                    production_rules[(*it).first].erase(remove(production_rules[(*it).first].begin(), production_rules[(*it).first].end(), production_rules[(*it).first][i]), production_rules[(*it).first].end());
                    i--;
                    break;
                }
                if (find(Reachable_symbols.begin(), Reachable_symbols.end(), (*it).first) == Reachable_symbols.end()) {
                    production_rules[(*it).first].clear();
                    production_rules.erase((*it).first);
                    deleted = true;
                    break;
                }
            }
            if(deleted){
                deleted = false;
                break;
            }
        }
    }

    // we zoeken de usefull symbols
    for (map<string, vector<vector<string>>>::iterator it = production_rules.begin(); it != production_rules.end(); it++) {
        if(production_rules[(*it).first].empty()){
            production_rules[(*it).first].clear();
            production_rules.erase((*it).first);
            continue;
        }
        Useful_symbols.push_back((*it).first);
        for (int i = 0; i < production_rules[(*it).first].size(); i++) {
            for (int j = 0; j < production_rules[(*it).first][i].size(); j++) {
                if (find(T.begin(), T.end(), production_rules[(*it).first][i][j]) != T.end()) {
                    if (find(Useful_symbols.begin(), Useful_symbols.end(), production_rules[(*it).first][i][j]) == Useful_symbols.end()) {
                        Useful_symbols.push_back(production_rules[(*it).first][i][j]);
                    }
                }
            }
        }
    }
    sort(Useful_symbols.begin(), Useful_symbols.end());


    int variablen_verwijdert = 0;
    int terminals_verwijdert = 0;
    for(int i = 0; i < V.size(); i++){
        if(find(Useful_symbols.begin(), Useful_symbols.end(), V[i]) == Useful_symbols.end()){
            variablen_verwijdert += 1;
            V.erase(remove(V.begin(), V.end(), V[i]), V.end());
            i--;
        }
    }
    for(int i = 0; i < T.size(); i++){
        if(find(Useful_symbols.begin(), Useful_symbols.end(), T[i]) == Useful_symbols.end()){
            terminals_verwijdert += 1;
            T.erase(remove(T.begin(), T.end(), T[i]), T.end());
            i--;
        }
    }


    second_amount = count_productions(production_rules);

    cout << " >> Eliminating useless symbols" << endl;
    cout << "  Generating symbols: {";
    if (!Generating_symbols.empty()) {
        for (int i = 0; i < Generating_symbols.size() - 1; i++) {
            cout << Generating_symbols[i] << ", ";
        }
        cout << Generating_symbols[Generating_symbols.size() - 1] << "}" << endl;
    }
    else{
        cout << "}" << endl;
    }
    cout << "  Reachable symbols: {";
    if (!Reachable_symbols.empty()) {
        for (int i = 0; i < Reachable_symbols.size() - 1; i++) {
            cout << Reachable_symbols[i] << ", ";
        }
        cout << Reachable_symbols[Reachable_symbols.size() - 1] << "}" << endl;
    }
    else{
        cout << "}" << endl;
    }
    cout << "  Useful symbols: {";
    if (!Reachable_symbols.empty()) {
        for (int i = 0; i < Useful_symbols.size() - 1; i++) {
            cout << Useful_symbols[i] << ", ";
        }
        cout << Useful_symbols[Useful_symbols.size() - 1] << "}" << endl;
    }
    else{
        cout << "}" << endl;
    }
    cout << "  Removed " << variablen_verwijdert << " variables, " << terminals_verwijdert << " terminals and " << first_amount - second_amount << " productions " << endl << endl;

    this->print();
    cout << endl;

    /// Replacing terminals in bad bodies
    vector<string> new_variables;
    first_amount = count_productions(production_rules);

    map<string, string> replacement;
    vector<string> terminals = T;
    // we gaan eerst kijken of de variabelen al bestaan die we nodig hebben
    for (map<string, vector<vector<string>>>::iterator it = production_rules.begin(); it != production_rules.end(); it++) {
        for (int i = 0; i < production_rules[(*it).first].size(); i++) {
            for (int j = 0; j < production_rules[(*it).first][i].size(); j++) {
                if(production_rules[(*it).first][i].size() == 1 && find(T.begin(), T.end(), production_rules[(*it).first][i][j]) != T.end()){
                    if(!replacement.count((*it).first)){
                        replacement[(*it).first] = "";
                    }
                    terminals.erase(remove(terminals.begin(), terminals.end(), production_rules[(*it).first][i][j]), terminals.end());
                    replacement[production_rules[(*it).first][i][j]] = (*it).first;
                }
            }
        }
    }

    //we gaan voor de overige terminals te bereiken nieuwe variabelen aanmaken

    for(int i = 0; i < terminals.size(); i++){
        new_variables.push_back("_" + terminals[i]);
        if(!replacement.count(terminals[i])){
            replacement[terminals[i]] = "";
        }
        replacement[terminals[i]] = "_" + terminals[i];
    }

    // we gaan de terminals vervangen door onze terminals
    for (map<string, vector<vector<string>>>::iterator it = production_rules.begin(); it != production_rules.end(); it++) {
        for (int i = 0; i < production_rules[(*it).first].size(); i++) {
            for (int j = 0; j < production_rules[(*it).first][i].size(); j++) {
                if(find(T.begin(), T.end(), production_rules[(*it).first][i][j]) != T.end()){
                    production_rules[(*it).first][i][j] = replacement[production_rules[(*it).first][i][j]];
                }
            }
        }
    }

    //we moeten een productie regel toevoegen voor de nieuwe variabelen
    for (int i = 0; i < new_variables.size(); i++) {
        vector<vector<string>> rules;
        vector<string> rule;
        rule.push_back(new_variables[i].substr(1,2));
        rules.push_back(rule);
        production_rules[new_variables[i]] = rules;
    }

    second_amount = count_productions(production_rules);

    V.insert(V.end(), new_variables.begin(), new_variables.end());
    sort(V.begin(), V.end());

    cout << " >> Replacing terminals in bad bodies" << endl;
    cout << "  Added "<< new_variables.size() << " new variables: {";
    if(!new_variables.empty()){
        for (int i = 0; i < new_variables.size()-1; i++) {
            cout << new_variables[i] << ", ";
        }
        cout << new_variables[new_variables.size()-1] << "}" << endl;
    }
    else{
        cout << "}" << endl;
    }

    cout << "  Created " << second_amount << " new productions, original had " << first_amount << endl << endl;

    this->print();
    cout << endl;

    /// Brake bodies and add new variables
    int broken_bodies = 0;
    int addad_variables = 0;
    new_variables.clear();

    for (map<string, vector<vector<string>>>::iterator it = production_rules.begin(); it != production_rules.end(); it++) {
        int counter = 2;
        for (int i = 0; i < production_rules[(*it).first].size(); i++) {
            if(production_rules[(*it).first][i].size() > 2){
                vector<vector<string>> rules;
                vector<string> rule;
                for (int j = 1; j < production_rules[(*it).first][i].size(); j++) {
                    rule.push_back(production_rules[(*it).first][i][j]);
                }
                rules.push_back(rule);
                string new_variable = (*it).first + "_" + to_string(counter);
                new_variables.push_back(new_variable);
                vector<string> rule_;
                rule_.push_back(production_rules[(*it).first][i][0]);
                rule_.push_back(new_variable);
                production_rules[(*it).first][i] = rule_;
                broken_bodies += 1;
                counter += 1;

                production_rules[new_variable] = rules;
                addad_variables += 1;
            }
        }
    }

    V.insert(V.end(), new_variables.begin(), new_variables.end());
    sort(V.begin(), V.end());

    cout << " >> Broke "<< broken_bodies <<" bodies, added " << addad_variables << " new variables" << endl;
    cout << ">>> Result CFG:" << endl << endl;
    this->print();

}
