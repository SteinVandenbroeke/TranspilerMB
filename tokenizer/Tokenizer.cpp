//
// Created by jens on 12/12/22.
//

#include "Tokenizer.h"

Tokenizer::Tokenizer(){
    types["string"] = "[STRINGTOKEN]";
    types["var"] = "[VARNAMETOKEN]";
    types["num"] = "[NUMBERTOKEN]";
    types["char"] = "[CHARTOKEN]";
    symbols = new ENFA(keywords);
    var = new ENFA("inputs/var.json");
}

Tokenizer::~Tokenizer() {
    delete symbols;
    delete var;
}

std::pair<int, std::string> Tokenizer::findSeparator(std::vector<std::string> lines, int lineNum, int linePos) {
    std::string separator;
    int p = linePos;
    for(p = linePos; p < lines[lineNum].size(); p++){
        separator+= lines[lineNum][p];
        if(std::count(separators.begin(), separators.end(), separator) == 0){
            separator.pop_back();
            p--;
            break;
        }
    }
    std::pair<int, std::string> pair;
    pair.first = p; pair.second = separator;
    return pair;
}

void Tokenizer::readAndSplit(const std::string &file) {
    split.clear();
    std::vector<std::string> lines;
    std::ifstream code(file);

    std::string line;
    while(std::getline(code, line)){
        if(line[0] == '/' && line[1] == '/'){ continue;}
        lines.push_back(line);
    }

    for(int i = 0; i < lines.size(); i++){
        std::string word;
        int linePos = 0;
        int newLinePos = -1;
        for(auto s : lines[i]){
            if(linePos <= newLinePos){
                linePos++;
                continue;
            }
            std::string temp;
            temp = s;

            if(std::count(separators.begin(), separators.end(), temp) > 0){
                if(!word.empty()){ split[i].push_back(word);}
                word = "";
                if( s == ' '){
                    linePos++;
                    continue;
                }
                std::pair<int, std::string> pair = findSeparator(lines, i, linePos);
                split[i].push_back(pair.second);
                newLinePos = pair.first;
                linePos++;
                continue;
            }
            word += s;
            if(s == lines[i].back()){
                split[i].push_back(word);
                word = "";
            }
            linePos++;
        }
    }
}

void Tokenizer::constructTokens() {
    if(split.empty()){
        return;
    }
    tokens.clear();

    for(const auto& i  : split){
        for(int j = 0; j < i.second.size(); j++){
            std::string check = i.second[j];
            if(symbols->accepts(check)){
                Token* token = new Token(check, "", i.first, j);
                tokens.push_back(token);
                continue;
            }
            std::pair<bool, std::vector<ENFA_State*>> p = var->acceptsHelper(check);
            if(p.first){
                std::string type;
                for(auto s : p.second){
                    if(s->isAccepting()){
                        std::string name = s->getName();
                        name.pop_back();
                        type = types[name];
                        break;
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
}

std::vector<Token *> Tokenizer::getTokens() {
    return tokens;
}

std::vector<Token *> Tokenizer::convert(const std::string& file) {
    readAndSplit(file);
    constructTokens();
    return getTokens();
}

void Tokenizer::displayWords() {
    for(const auto& i : split){
        std::cout << i.first << ": ";
        for(const auto& j : i.second){
            std::cout << j << " | ";
        }
        std::cout << std::endl;
    }
}

void Tokenizer::displayTokens() {
    for(auto token : tokens){
        token->print();
    }
}