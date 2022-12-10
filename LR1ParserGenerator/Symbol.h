//
// Created by kobedb on 7/12/22.
//

#ifndef LR_PARSING__SYMBOL_H
#define LR_PARSING__SYMBOL_H

#include <iostream>
#include <vector>
#include "ImmutableHashable.h"

namespace LR1ParserGeneratorStuff {
    struct Symbol {
        std::string name;
        bool b_isTerminal;
        Symbol(std::string name, bool isTerminal) : name{std::move(name)}, b_isTerminal{isTerminal} {}
        bool isTerminal() const { return b_isTerminal; }
        bool isNonterminal() const { return !b_isTerminal; }
    };

    inline std::ostream& operator<<(std::ostream& os, const Symbol& symb) {
        os << symb.name;
        return os;
    }

    struct Production {
        Symbol* head;
        std::vector<Symbol*> body;
        Production(Symbol* head, std::vector<Symbol*> body) : head{head}, body{std::move(body)} {}
    };

    inline std::ostream& operator<<(std::ostream& os, const Production& prod) {
        os << *prod.head << " -> ";
        for(Symbol* s : prod.body) {
            os << *s << " ";
        }
        return os;
    }

    // An ugly construction to emulate rust enums
    struct SymbolOrEps : public ImmutableHashable {
        enum Discr {
            Symbol, Eps
        };
        Discr discriminant{};
        struct Symbol* symbol{};
        SymbolOrEps(Discr d, struct Symbol* s) : ImmutableHashable{}, discriminant{d}, symbol{s} {
            if(d == Eps) {
                ImmutableHashable::setStringRepr("<EPS>");
                return;
            }
            if(s) {
                ImmutableHashable::setStringRepr(s->name);
            }
        }
    };

    inline SymbolOrEps makeSymbol(Symbol* s) { return SymbolOrEps{SymbolOrEps::Symbol, s};}
    inline SymbolOrEps makeEps() { return SymbolOrEps{SymbolOrEps::Eps, nullptr}; }
}

#endif //LR_PARSING__SYMBOL_H
