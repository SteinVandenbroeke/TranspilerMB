//
// Created by jens on 12/12/22.
//

#include <vector>
#include <set>
#include <string>

#include "ENFA.h"

#ifndef TRANSPILER_TOKENIZER_H
#define TRANSPILER_TOKENIZER_H

class Tokenizer {
private:
    std::vector<std::string> keywords = {"while", "if", "else", "print"};
    std::vector<std::string> separators = {"{", "}", "(", ")", ";"};
    std::vector<std::string> operations = {"*", "/", "+", "-", "<", "<=", ">=", ">", "==", "!="};
    std::vector<std::string> variables = {"int", "double", "bool"};
};


#endif //TRANSPILER_TOKENIZER_H
