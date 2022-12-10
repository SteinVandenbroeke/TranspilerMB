//
// Created by kobedb on 7/12/22.
//

#ifndef LR_PARSING__GRAMMAR_H
#define LR_PARSING__GRAMMAR_H

#include "json.hpp"

#include "Symbol.h"
#include "Item.h"

#include <unordered_map>
#include <unordered_set>
#include "kdb_facilities.h"

namespace LR1ParserGeneratorStuff {

    using SymbolOrEpsSet = std::unordered_set<SymbolOrEps, HashableHashFunc>;
    using FirstSets = std::unordered_map<Symbol*, SymbolOrEpsSet>;

    struct Grammar {

        std::vector<std::unique_ptr<Symbol>> symbols;
        std::unordered_map<std::string, Symbol*> symbolTable;
        std::unordered_map<Symbol*, std::vector<std::unique_ptr<Production>>> productionTable;
        Symbol* start;

        FirstSets m_firstSets;

        explicit Grammar(const nlohmann::json &cfgDesc) {
            for(const auto& v : cfgDesc["Variables"]) {
                auto s = std::make_unique<Symbol>(v, false);
                symbolTable.emplace(v, s.get());
                symbols.emplace_back(std::move(s));
            }
            for(const auto& t : cfgDesc["Terminals"]) {
                auto s = std::make_unique<Symbol>(t, true);
                symbolTable.emplace(t, s.get());
                symbols.emplace_back(std::move(s));
            }

            for(const auto& p : cfgDesc["Productions"]) {
                std::vector<Symbol*> body{};
                for(const auto& bs : p["body"]) {
                    body.push_back(symbolTable.at(bs));
                }
                auto headSymbol = symbolTable.at(p["head"]);
                productionTable[headSymbol].emplace_back(std::make_unique<Production>(headSymbol, body));
            }

            // Augmentation
            auto originalStart = symbolTable.at(cfgDesc["Start"]);
            auto augmentedStartName = originalStart->name + "'";

            auto augmentedStart = std::make_unique<Symbol>(augmentedStartName, false);
            auto augmentedStartPtr = augmentedStart.get();
            symbols.push_back(std::move(augmentedStart));

            symbolTable.emplace(augmentedStartName, augmentedStartPtr);

            productionTable[augmentedStartPtr].emplace_back(
                    std::make_unique<Production>(augmentedStartPtr, std::vector<Symbol*>{originalStart})
                    );

            start = augmentedStartPtr;

            // EOS symbol
            auto eos = std::make_unique<Symbol>("$", true);
            symbolTable.emplace(eos->name, eos.get());
            symbols.push_back(std::move(eos));

            m_firstSets = firstSets();
        }

        Symbol* getSymbol(const std::string& name) const { return symbolTable.at(name); }
        const std::vector<std::unique_ptr<Production>>& getProductionsOf(Symbol* s) const { return productionTable.at(s);}

        //Grammar(const Grammar &other) = delete;

        Symbol *getStart() const { return start; }

        const std::vector<std::unique_ptr<Symbol>>& getSymbols() const {return symbols;}

        FirstSets firstSets() {
            Grammar& g = *this;
            FirstSets first_sets;
            for(const auto& t : g.getSymbols()) {
                if(t->isTerminal()) {
                    //first_sets.insert(t.get(), std::unordered_set<SymbolOrEps, ImmutableHashable>{SymbolOrEps{SymbolOrEps::Symbol, nullptr}});
                    first_sets[t.get()] = {makeSymbol(t.get())};
                }
            }

            bool progressMade = true;
            while(progressMade) {
                progressMade = false;
                for (const auto &nt: g.getSymbols()) {
                    if (nt->isNonterminal()) {
                        for (const auto &prod : g.getProductionsOf(nt.get())) {
                            auto body = prod->body;
                            if (body.empty()) {
                                progressMade |= first_sets[nt.get()].insert(makeEps()).second;
                                continue;
                            }
                            bool prevVarsContainEpsilonUntilNow = true;
                            for (size_t i = 0; i < body.size() && prevVarsContainEpsilonUntilNow; ++i) {
                                auto first_of_ith_symbol = first_sets[body[i]];
                                if (!KDBFacilities::contains(first_of_ith_symbol, makeEps())) {
                                    prevVarsContainEpsilonUntilNow = false;
                                }
                                first_of_ith_symbol.erase(makeEps());

                                size_t prevSize = first_sets[nt.get()].size();
                                first_sets[nt.get()].insert(first_of_ith_symbol.begin(), first_of_ith_symbol.end());
                                if (prevSize != first_sets[nt.get()].size()) {
                                    progressMade = true;
                                }
                            }
                            if (prevVarsContainEpsilonUntilNow) {
                                progressMade |= first_sets[nt.get()].insert(makeEps()).second;
                            }
                        }
                    }
                }
            }
            return first_sets;
        }

    public:

        SymbolOrEpsSet first(const std::vector<Symbol*>& grammar_string) const {
            if(grammar_string.empty()) {
                return {makeEps()};
            }
            SymbolOrEpsSet result;
            bool prevVarsContainEpsilonUntilNow = true;
            for (size_t i = 0; i < grammar_string.size() && prevVarsContainEpsilonUntilNow; ++i) {
                auto first_of_ith_symbol = m_firstSets.at(grammar_string[i]);
                if (!KDBFacilities::contains(first_of_ith_symbol, makeEps())) {
                    prevVarsContainEpsilonUntilNow = false;
                }
                first_of_ith_symbol.erase(makeEps());

                result.insert(first_of_ith_symbol.begin(), first_of_ith_symbol.end());
            }
            if (prevVarsContainEpsilonUntilNow) {
                result.insert(makeEps());
            }
            return result;
        }
    };
}

#endif //LR_PARSING__GRAMMAR_H
