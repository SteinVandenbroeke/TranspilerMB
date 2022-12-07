//
// Created by stein on 29/11/2022.
//

#include "Token.h"

Token::Token(std::string type): type(type), text(type) {

}

Token::Token(std::string type, std::string text): type(type), text(text) {

}

std::string Token::getText() {
    return text;
}

std::string Token::getType() {
    return type;
}
