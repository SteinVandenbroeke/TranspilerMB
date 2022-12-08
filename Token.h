//
// Created by stein on 29/11/2022.
//

#ifndef TRANSPILER_TOKEN_H
#define TRANSPILER_TOKEN_H


#include <string>

class Token {
std::string type;
std::string text;
int lineCount;
public:
    Token(std::string type);
    Token(std::string type, std::string text);
    std::string getText();
    std::string getType();
    int getLine();
};


#endif //TRANSPILER_TOKEN_H
