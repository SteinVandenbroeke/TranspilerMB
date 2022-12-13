//
// Created by jens on 12/12/22.
//

#include "Tokenizer.h"

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
                split[i].push_back(word);
                word = "";
                if( s == ' '){ continue;}
                word += s;
                split[i].push_back(word);
                word = "";
                continue;
            }
            word += s;
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