#include <iostream>
#include <fstream>

#include "LR1ParseTableGenerator.h"


int main() {
    std::string pathToGrammarJson = "inputs/staal_fixed.json";

    std::ifstream input(pathToGrammarJson);
    if(!input) {
        std::cerr << "CFG-constructor: can't open json-input file: " << pathToGrammarJson << "\n";
    }
    nlohmann::json jg;
    input >> jg;

    auto lr1ParseTableGenerator = LR1ParserGeneratorStuff::LR1ParseTableGenerator{jg};
    lr1ParseTableGenerator.writeTableToJson(pathToGrammarJson + "_PARSETABLE.json");
    std::cout << lr1ParseTableGenerator.getLR1ParseTable() << "\n";

    return 0;
}
