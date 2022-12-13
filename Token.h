//
// Created by stein on 29/11/2022.
//

#ifndef TRANSPILER_TOKEN_H
#define TRANSPILER_TOKEN_H


#include <string>
#include <iostream>

class Token {
std::string type;
std::string text;
int lineNum;
int linePos;

public:
    Token(std::string type);
    Token(std::string type, std::string text);
    Token(std::string type, std::string text, int lineNum, int linePos);

    std::string getText();
    std::string getType();
    int getLine();
    int getLinePos();

    void print();
};


#endif //TRANSPILER_TOKEN_H
