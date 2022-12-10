//
// Created by kobedb on 9/12/22.
//

#ifndef LR_PARSING__LR1PARSETABLEGENERATOR_H
#define LR_PARSING__LR1PARSETABLEGENERATOR_H

#include "Grammar.h"
#include "lr1_parse_table_gen.h"

namespace LR1ParserGeneratorStuff {

    class LR1ParseTableGenerator {
        Grammar augmentedGrammar;
        LR1ParseTable parseTable;

    public:
        explicit LR1ParseTableGenerator(const nlohmann::json& grammarDesc);

        void writeTableToJson(const std::string& pathToOutputFile) const;

        const LR1ParseTable& getLR1ParseTable() const { return parseTable; }
    };

} // LR1ParserGeneratorStuff

#endif //LR_PARSING__LR1PARSETABLEGENERATOR_H
