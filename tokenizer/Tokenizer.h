//
// Created by jens on 12/12/22.
//

#include <vector>
#include <set>
#include <map>
#include <string>
#include <iostream>
#include <fstream>

#include "ENFA.h"
#include "Token.h"

#ifndef TRANSPILER_TOKENIZER_H
#define TRANSPILER_TOKENIZER_H

class Tokenizer {
private:
    std::vector<std::string> separators = {" ", ";", "(", ")", "=", "!=", "<=", ">=", "<", ">", "==", "{", "}", "+", "-", "*", "/"};
    std::vector<std::string> keywords = {";", "print", "int", "double", "char", "string", "=", "while", "if", "(", ")", "<=", ">=", "<", ">", "==", "{", "}", "+", "-", "*", "/"};
    std::map<std::string, std::string> types;
    ENFA* symbols;
    ENFA* var;
    std::map<int, std::vector<std::string>> split;
    std::vector<Token*> tokens;
public:
    Tokenizer();

    ~Tokenizer();

    std::pair<int, std::string> findSeparator(std::vector<std::string> lines, int lineNum, int linePos);

    std::pair<int, std::string> read(std::vector<std::string> lines, int lineNum, int linePos);

    void readAndSplit(const std::string& file);

    void constructTokens();

    std::vector<Token*> getTokens();

    std::vector<Token*> convert(const std::string& file);

    void displayWords();

    void displayTokens();
};


#endif //TRANSPILER_TOKENIZER_H
