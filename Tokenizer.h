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

#ifndef TRANSPILER_TOKENIZER_H
#define TRANSPILER_TOKENIZER_H

class Tokenizer {
private:
    std::vector<std::string> keywords = {"while", "if", "else", "print"};
    std::vector<char> separators = {' ', '{', '}', '(', ')', ';', '"', '\''};
    std::vector<std::string> operations = {"*", "/", "+", "-", "<", "<=", ">=", ">", "==", "!="};
    std::vector<std::string> variables = {"int", "double", "bool", "string"};

public:
    Tokenizer(){}

    void convert(std::string file);
};


#endif //TRANSPILER_TOKENIZER_H
