//
// Created by stein on 1/12/2022.
//

#ifndef TRANSPILER_CSTNODE_H
#define TRANSPILER_CSTNODE_H
#include "Token.h"
#include <vector>
#include "json.hpp"
#include <fstream>
using nlohmann::json;

class CSTNode {
public:
    CSTNode(Token* token);
    const std::vector<CSTNode *> &getChildren() const;
    Token *getToken() const;
    CSTNode* removeChild(CSTNode* node);
    void addChild(CSTNode* node);
private:
    std::vector<CSTNode*> children;
    Token* token;
};

class CST{
public:
    CST(std::vector<Token*>& tokens, const std::string& parseTable);
private:
    CSTNode* root;
};


#endif //TRANSPILER_CSTNODE_H
