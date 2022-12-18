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
    explicit Token(std::string type); //testing only
    Token(std::string type, int lineNum, int linePos);
    Token(std::string type, std::string text); //testing only
    Token(std::string type, std::string text, int lineNum, int linePos);

    std::string getText();
    std::string getType();
    int getLine() const;
    int getLinePos() const;

    void print();
};


#endif //TRANSPILER_TOKEN_H
