//
// Created by kobedb on 9/12/22.
//

#include "LR1ParseTableGenerator.h"
#include <iostream>
#include "lr1_parse_table_gen.h"

namespace LR1ParserGeneratorStuff {

    LR1ParserGeneratorStuff::LR1ParseTableGenerator::LR1ParseTableGenerator(const nlohmann::json& grammarDesc) : augmentedGrammar{grammarDesc} {
        parseTable = calcLR1ParseTable(augmentedGrammar);
    }

    void LR1ParserGeneratorStuff::LR1ParseTableGenerator::writeTableToJson(const std::string &pathToOutputFile) const {
        nlohmann::json jsonTable;

        jsonTable["Variables"] = std::vector<std::string>{};
        for(const auto& variable : parseTable.nonterminals) {
            jsonTable["Variables"].push_back(variable->name);
        }

        jsonTable["Terminals"] = std::vector<std::string>{};
        for(const auto& terminal : parseTable.terminals) {
            jsonTable["Terminals"].push_back(terminal->name);
        }

        jsonTable["NumberOfProductions"] = parseTable.productions.size();
        jsonTable["Productions"] = std::vector<nlohmann::json>{};
        for(int i = 0; i < parseTable.productions.size(); ++i) {
            nlohmann::json production;
            production["index"] = i;
            production["head"] = parseTable.productions[i]->head->name;
            production["body"] = std::vector<std::string>{};
            for(const auto& symbol : parseTable.productions[i]->body) {
                production["body"].push_back(symbol->name);
            }
            jsonTable["Productions"].push_back(production);
        }

        jsonTable["ActionTable"] = std::vector<nlohmann::json>{};
        for(const auto& actionp : parseTable.actions) {
            const auto& key = actionp.first;
            const auto& actionDesc = actionp.second;
            nlohmann::json action;
            action["StateIndex"] = key.first;
            action["Input"] = key.second->name;
            switch(actionDesc.type) {
                case Action::Shift: {
                    action["ActionType"] = "s";
                    action["ActionArgument"] = actionDesc.stateToShift;
                } break;
                case Action::Reduce: {
                    action["ActionType"] = "r";
                    action["ActionArgument"] = actionDesc.production;
                } break;
                case Action::Accept: action["ActionType"] = "acc"; break;
            }
            jsonTable["ActionTable"].push_back(action);
        }

        jsonTable["GotoTable"] = std::vector<nlohmann::json>{};
        for(const auto& gotop : parseTable.gotos) {
            const auto& key = gotop.first;
            nlohmann::json jsonGoto;
            jsonGoto["StateIndex"] = key.first;
            jsonGoto["Input"] = key.second->name;
            jsonGoto["GotoIndex"] = gotop.second;
            jsonTable["GotoTable"].push_back(jsonGoto);
        }

        std::ofstream ofs{pathToOutputFile};
        ofs << std::setw(4) << jsonTable << "\n";
    }
} // LR1ParserGeneratorStuff