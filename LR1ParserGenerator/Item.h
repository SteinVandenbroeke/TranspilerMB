//
// Created by kobedb on 7/12/22.
//

#ifndef LR_PARSING__ITEM_H
#define LR_PARSING__ITEM_H

#include "Grammar.h"
#include "ImmutableHashable.h"

namespace LR1ParserGeneratorStuff {

    /// @immutable
    struct Item : public ImmutableHashable {
        Item(Production* prod, int dotPosition, Symbol* lookahead)
                :   ImmutableHashable { std::to_string((long)prod) + std::to_string(dotPosition) + std::to_string((long)lookahead)},
                    production{prod},
                    dotPosition{dotPosition},
                    lookahead{lookahead} {
            if (dotPosition < 0 || (size_t)dotPosition > production->body.size()) {
                std::cerr << "Item(): Invalid dot position\n";
            }
        }

        bool hasNonterminalAfterDot() const {
            return (size_t)dotPosition < production->body.size() && production->body.at(dotPosition)->isNonterminal();
        }

        bool hasTerminalAfterDot() const {
            return (size_t)dotPosition < production->body.size() && production->body.at(dotPosition)->isTerminal();
        }

        bool hasSymbolAfterDot() const { return hasNonterminalAfterDot() || hasTerminalAfterDot(); }

        Symbol* symbolAfterDot() const {
            return production->body.at(dotPosition);
        }

        std::vector<Symbol*> symbolsAfterPosition(int pos) const {
            if((size_t)pos > production->body.size()) { std::cerr << "symbolsAfterPosition: pos: " << pos << " out of bounds\n";}
            std::vector<Symbol*> result;
            for(size_t i = pos; i < production->body.size(); ++i) {
                result.push_back(production->body[i]);
            }
            return result;
        }

        Production* getProduction() const { return production; }
        int getDotPosition() const { return dotPosition; }
        Symbol* getLookahead() const { return lookahead; }

        //bool operator==(const Item& other) { return production == other.production && dotPosition == other.dotPosition && lookahead == other.lookahead; }

        friend std::ostream& operator<<(std::ostream& os, const Item& item);
    private:
        Production* production;
        int dotPosition; // position of the next symbol to be parsed
        Symbol* lookahead;
    };

    inline std::ostream& operator<<(std::ostream& os, const Item& item) {
        os << *item.production << " | " << item.dotPosition << " | " << *item.lookahead;
        return os;
    }

} // LR1ParserGeneratorStuff

#endif //LR_PARSING__ITEM_H
