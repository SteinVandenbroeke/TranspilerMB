//
// Created by jens on 12/12/22.
//

#include "Tokenizer.h"

Tokenizer::Tokenizer(){
    types["string"] = "[STRINGTOKEN]";
    types["var"] = "[VARNAMETOKEN]";
    types["num"] = "[NUMBERTOKEN]";
    types["char"] = "[CHARTOKEN]";
}

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
            std::string check = i.second[j];
            if(symbols.accepts(check)){
                Token* token = new Token(check, "", i.first, j);
                tokens.push_back(token);
                continue;
            }
            std::pair<bool, std::vector<ENFA_State*>> p = var.acceptsHelper(check);
            if(p.first){
                std::string type;
                for(auto s : p.second){
                    if(s->isAccepting()){
                        std::string name = s->getName();
                        name.pop_back();
                        type = types[name];
                    }
                }
                Token* token = new Token(type, check, i.first, j);
                tokens.push_back(token);
                continue;
            }

        }
    }
    Token* endOfString = new Token("$","$", tokens.back()->getLine(), tokens.back()->getLinePos()+1);
    tokens.push_back(endOfString);
    return tokens;
}