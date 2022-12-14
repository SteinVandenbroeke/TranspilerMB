//
// Created by stein on 29/11/2022.
//

#include "Token.h"

Token::Token(std::string type): type(type), text(type) {

}

Token::Token(std::string type, std::string text): type(type), text(text) {

}

Token::Token(std::string type, std::string text, int lN, int lP): type(type), text(text), lineNum(lN), linePos(lP) {

}

std::string Token::getText() {
    return text;
}

std::string Token::getType() {
    return type;
}

int Token::getLine() {
    return lineNum;
}

int Token::getLinePos(){
    return linePos;
}

void Token::print(){
    std::cout << "{" << type << ": " << text << " at (" << lineNum << ", " << linePos << ")}" << std::endl;
}