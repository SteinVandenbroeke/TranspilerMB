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
    std::vector<std::string> keywords = {";", "int", "double", "char", "string", "=", "while", "if", "(", ")", "<=", ">=", "<", ">", "==", "{", "}", "+", "-", "*", "/"};
public:
    Tokenizer()= default;

    std::vector<Token*> convert(const std::string& file);
};


#endif //TRANSPILER_TOKENIZER_H
