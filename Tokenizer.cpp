//
// Created by jens on 12/12/22.
//

#include "Tokenizer.h"

std::vector<Token*> Tokenizer::convert(const std::string& file) {
    std::vector<std::string> lines;
    std::map<int, std::vector<std::string>> split;

    std::vector<Token*> tokens;

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

    ENFA symbols = ENFA(keywords);
    ENFA var = ENFA("inputs/var.json");
    for(const auto& i  : split){
        for(int j = 0; j < i.second.size(); j++){
            if(symbols.accepts(i.second[j])){
                Token* token = new Token(i.second[j], "", i.first, j);
                tokens.push_back(token);
                continue;
            }
            if(var.accepts(i.second[j])){
                std::string type;
                Token* token = new Token(type, i.second[j], i.first, j);
                tokens.push_back(token);
                continue;
            }

        }
    }
    return tokens;

}