#include <iostream>
#include <fstream>

#include "LR1ParseTableGenerator.h"


int main(int argc, char** argv) {
    if(argc != 2) {
        std::cerr << "Expected 1 argument: path to grammar.json";
    }

    std::string pathToGrammarJson { argv[1] };
    std::cout << "Generating parse table for " << pathToGrammarJson << "\n";

    std::ifstream input(pathToGrammarJson);
    if(!input) {
        std::cerr << "CFG-constructor: can't open json-input file: " << pathToGrammarJson << "\n";
    }

    std::stringstream inputStr;
    inputStr << input.rdbuf();
    std::cout << inputStr.str() << "\n";

    nlohmann::json jg;
    inputStr >> jg;

    auto lr1ParseTableGenerator = LR1ParserGeneratorStuff::LR1ParseTableGenerator{jg};
    lr1ParseTableGenerator.writeTableToJson(pathToGrammarJson + "_PARSETABLE.json");
    std::cout << lr1ParseTableGenerator.getLR1ParseTable() << "\n";

    return 0;
}
