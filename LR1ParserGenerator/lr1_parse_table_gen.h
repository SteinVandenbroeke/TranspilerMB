//
// Created by kobedb on 7/12/22.
//

#ifndef LR_PARSING__LR1_PARSE_TABLE_GEN_H
#define LR_PARSING__LR1_PARSE_TABLE_GEN_H

#include "Grammar.h"
#include "Item.h"
#include <unordered_set>
#include <fstream>
#include "kdb_facilities.h"

using KDBFacilities::contains;

namespace LR1ParserGeneratorStuff {

    using ItemSet = std::unordered_set<Item, HashableHashFunc>;

    inline std::ostream& operator<<(std::ostream& os, const ItemSet& itemSet) {
        for(const auto& item : itemSet) {
            os << item << "\n";
        }
        return os;
    }

    inline ItemSet lr1Closure(const ItemSet& I, const Grammar& augmentedGrammar) {
        auto closureSet { I };
        bool newItemsAdded { true };
        while(newItemsAdded) {
            newItemsAdded = false;
            for(const auto& item : closureSet) {
                if(item.hasNonterminalAfterDot()) {
                    Symbol* B = item.symbolAfterDot();
                    for(const auto& BProd : augmentedGrammar.getProductionsOf(B)) {
                        for(auto b : augmentedGrammar.first(concat(item.symbolsAfterPosition(item.getDotPosition()+1), {item.getLookahead()}))) {
                            if(b.discriminant == SymbolOrEps::Symbol) {
                                auto result = closureSet.insert(Item{BProd.get(), 0, b.symbol});
                                newItemsAdded |= result.second;
                            }
                        }
                    }
                }
            }
        }

        return closureSet;
    }

    inline ItemSet lr1Goto(const ItemSet& I, Symbol* X, const Grammar& augmentedGrammar) {
        ItemSet J{};

        for(const auto& item : I) {
            if(item.hasSymbolAfterDot() && item.symbolAfterDot()->name == X->name) {
                J.insert(Item{ item.getProduction(), item.getDotPosition()+1, item.getLookahead() });
            }
        }

        return lr1Closure(J, augmentedGrammar);
    }

    struct LR1Collection {
        std::vector<ItemSet> states;
        std::unordered_map<std::pair<int, Symbol*>, int, pair_hash> transitions; // the goto function maps (state_i, nonterminal) -> state_j
    };

    inline LR1Collection calcLR1Collection(const Grammar& g) {
        const auto& augmentedStartProduction = g.getProductionsOf(g.getStart()).at(0);
        Item startItem = Item{ augmentedStartProduction.get(), 0, g.getSymbol("$")};

        LR1Collection collection{};
        collection.states.push_back(lr1Closure({startItem}, g));

        bool progressMade {true};
        while(progressMade) {
            progressMade = false;
            for(size_t i = 0; i < collection.states.size(); ++i) {
                for(const auto& X : g.getSymbols()) {
                    auto stateToGoTo = lr1Goto(collection.states[i], X.get(), g);
                    if(!stateToGoTo.empty()) {
                        auto stateToGoToIndex = KDBFacilities::find(collection.states, stateToGoTo);
                        if(stateToGoToIndex >= 0) {
                            if(collection.transitions.find({i, X.get()}) == collection.transitions.end()) {
                                progressMade = true;
                                collection.transitions[{i, X.get()}] = stateToGoToIndex;
                            }
                        }
                        else {
                            progressMade = true;
                            collection.states.push_back(stateToGoTo);
                            collection.transitions[{i, X.get()}] = collection.states.size()-1;
                            //std::cout << stateToGoTo << "\n\n";
                        }
                    }
                }
            }
        }
        return collection;
    }

    struct Action {
        enum Type {
            Shift, Reduce, Accept
        };
        Type type{};
        int stateToShift{};
        int production{}; // index in vector parseTable.productions

        bool operator==(const Action& other) const { return other.type == this->type && other.stateToShift == this->stateToShift && other.production == this->production; }
        bool operator!=(const Action& other) const { return !(other == *this); }
    };

    inline std::ostream& operator<<(std::ostream& os, const Action& action) {
        switch(action.type) {
            case Action::Shift: os << "s" << action.stateToShift; break;
            case Action::Reduce: os << "r" << action.production; break;
            case Action::Accept: os << "acc"; break;
        }
        return os;
    }

    struct LR1ParseTable {
        // No entry in the table means ERROR
        std::unordered_map<std::pair<int, Symbol*>, Action, pair_hash> actions;
        std::unordered_map<std::pair<int, Symbol*>, int, pair_hash> gotos;
        int numOfStates{}; // State indexes start counting from 0
        std::vector<Symbol*> terminals; // state_index + terminal -> key for ACTION table
        std::vector<Symbol*> nonterminals; // state_index + variable -> key for GOTO table
        std::vector<Production*> productions;
    };

    inline void reportConflict(const std::pair<int,Symbol*> key, const LR1ParseTable& parseTable) {
        std::cerr << "calcLR1ParseTable: action conflict for (" << key.first << ", " << key.second->name << ")\t";
        std::cerr << "Overwritten value: " << parseTable.actions.at(key) << "\n";
    }

    inline LR1ParseTable calcLR1ParseTable(const Grammar& augmentedGrammar) {
        using namespace KDBFacilities;

        auto lr1Collection = calcLR1Collection(augmentedGrammar);

        LR1ParseTable parseTable{};
        parseTable.numOfStates = lr1Collection.states.size();

        // Fill in parseTable.terminals + parseTable.nonteminals + parseTable.productions
        for(const auto& symbol : augmentedGrammar.getSymbols()) {
            if(symbol->isNonterminal()) {
                if(symbol.get() != augmentedGrammar.getStart()) {
                    parseTable.nonterminals.push_back(symbol.get());

                    // add its productions to parseTable's productions
                    const auto& symbolProds = augmentedGrammar.getProductionsOf(symbol.get());
                    for(const auto& symbolProd : symbolProds) {
                        parseTable.productions.push_back(symbolProd.get());
                    }
                }
            }
            else {
                // symbol is a terminal
                parseTable.terminals.push_back(symbol.get());
            }
        }

        // Fill in ACTION table
        for(size_t i = 0; i < lr1Collection.states.size(); ++i) {
            for(const auto& item : lr1Collection.states[i]) {
                if(item.hasTerminalAfterDot()) {
                    auto key = std::pair<int,Symbol*>{i, item.symbolAfterDot()};
                    assert(hasEntryForKey(lr1Collection.transitions, key));
                    auto action = Action{Action::Shift, lr1Collection.transitions[key]};
                    if(hasEntryForKey(parseTable.actions, key) && parseTable.actions.at(key) != action) {
                        reportConflict(key, parseTable);
                    }
                    parseTable.actions[key] = Action{Action::Shift, lr1Collection.transitions[key]};
                }
                if(!item.hasSymbolAfterDot()) {
                    if(item.getProduction()->head == augmentedGrammar.getStart()) {
                        auto key = std::pair<int,Symbol*>{i, augmentedGrammar.getSymbol("$")};
                        if(hasEntryForKey(parseTable.actions, key)) {
                            reportConflict(key, parseTable);
                        }
                        parseTable.actions[key] = Action{Action::Accept};
                    }
                    else {
                        auto key = std::pair<int,Symbol*>{i, item.getLookahead()};
                        if(hasEntryForKey(parseTable.actions, key)) {
                            reportConflict(key, parseTable);
                        }
                        parseTable.actions[key] = Action{
                            Action::Reduce,
                            -1,
                            KDBFacilities::find(parseTable.productions, item.getProduction()),
                            };
                    }
                }
            }
        }

        // Fill in GOTO table
        for(const auto& transition : lr1Collection.transitions) {
            int fromStateNumber = transition.first.first;
            Symbol* inputSymbolForTransition = transition.first.second;
            int toState = transition.second;
            if(inputSymbolForTransition->isNonterminal()) {
                parseTable.gotos[{fromStateNumber, inputSymbolForTransition}] = toState;
            }
        }
        return parseTable;
    }

    inline std::ostream& operator<<(std::ostream& os, const LR1ParseTable& parseTable) {
        using namespace KDBFacilities;

        os << "Productions:\n\n";
        for(int i = 0; i < parseTable.productions.size(); ++i) {
            os << i << ":\t" << *parseTable.productions[i] << "\n";
        }

        os << "\n\nTable:\n\n";

        for(int i = 0; i < parseTable.numOfStates; ++i) {
            os << i << "\t|\t";
            for(const auto& terminal : parseTable.terminals) {
                if(hasEntryForKey(parseTable.actions, {i, terminal})) {
                    const auto& action = parseTable.actions.at({i,terminal});
                    switch(action.type) {
                        case Action::Shift:  os << "s" << action.stateToShift << "\t"; break;
                        case Action::Reduce: os << "r" << action.production << "\t"; break;
                        case Action::Accept: os << "acc" << "\t"; break;
                    }
                }
                else {
                    os << "-" << "\t";
                }
            }
            for(const auto& nonterminal : parseTable.nonterminals) {
                if(hasEntryForKey(parseTable.gotos, {i, nonterminal})) {
                    const auto& gotoEntry = parseTable.gotos.at({i,nonterminal});
                    os << gotoEntry << "\t";
                }
                else {
                    os << "-" << "\t";
                }
            }
            os << "\n";
        }
        return os;
    }
}

#endif //LR_PARSING__LR1_PARSE_TABLE_GEN_H
