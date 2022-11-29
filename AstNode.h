//
// Created by stein on 29/11/2022.
//

#ifndef TRANSPILER_ASTNODE_H
#define TRANSPILER_ASTNODE_H
#include "vector"
#include "Token.h"
#include "string"

class AstNode {
    std::vector<AstNode*> childeren;
    Token* token;
    bool containsCode();
public:
    AstNode(Token* token);
    std::string getTokenType();
    std::string getTokenText();
    std::string getJsCode();
};


#endif //TRANSPILER_ASTNODE_H
