//
// Created by jens on 12/12/22.
//

#include "Tokenizer.h"

ENFA createENFA(const std::vector<std::string>& words){
    ENFA temp = ENFA();
    std::map<std::string, ENFA_State*> tempStates;
    std::vector<ENFA_State*> tempFinal;
    std::set<char> alfabet;
    auto* start = new ENFA_State("start", true, false);
    tempStates[start->getName()] = start;
    int name = 0;
    for(auto w : words){
        ENFA_State* current = start;
        for(auto c : w){
            alfabet.insert(c);
            auto* next = new ENFA_State(std::to_string(name), false, false);
            if(c == w.back()){
                next->setAccepting(true);
                tempFinal.push_back(next);
            }
            tempStates[next->getName()] = next;
            current->setNextState(c, next);
            current = next;
        }
    }
    temp.setStates(tempStates);
    temp.setCurrentStates({start});
    temp.setFinalStates(tempFinal);
    temp.setStartState(start);
    std::vector<char> alphabet(alfabet.begin(), alfabet.end());
    temp.setAlfabet(alphabet);
    for(const auto& w : words){
        if(!temp.accepts(w)){
            std::cout << "you fucked up" << std::endl;
        }
    }
    return temp;
}

void Tokenizer::convert(const std::string& file) {
    std::vector<std::string> lines;
    std::map<int, std::vector<std::string>> split;

    std::ifstream code(file);

    std::string line;
    while(std::getline(code, line)){
        lines.push_back(line);
    }

    for(int i = 0; i < lines.size(); i++){
        std::string word;
        for(auto s : lines[i]){
            std::string temp;
            temp = s;
            if(std::count(separators.begin(), separators.end(), temp) > 0){
                if(!word.empty()){ split[i].push_back(word);}
                word = "";
                if( s == ' '){ continue;}
                word += s;
                split[i].push_back(word);
                word = "";
                continue;
            }
            word += s;
            if(s == lines[i].back()){
                split[i].push_back(word);
                word = "";
            }
        }
    }

    for(const auto& i : split){
        std::cout << i.first << ": ";
        for(const auto& j : i.second){
            std::cout << j << " | ";
        }
        std::cout << std::endl;
    }
}