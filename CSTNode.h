//
// Created by stein on 1/12/2022.
//

#ifndef TRANSPILER_CSTNODE_H
#define TRANSPILER_CSTNODE_H
#include "tokenizer/Token.h"
#include <vector>
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>
using nlohmann::json;

class CSTNode {
public:
    ~CSTNode();
    const std::vector<CSTNode *> &getChildren() const;
    CSTNode* removeChild(CSTNode* node);
    void addChild(CSTNode* node);
private:
    std::vector<CSTNode*> children;
};

class LeafNode : public CSTNode {
public:
    LeafNode(Token* token);
    Token *getToken() const;
private:
    Token* token;
};

class InternalNode : public CSTNode{
public:
    const std::string &getValue() const;
    InternalNode(const std::string& value);
private:
    std::string value;
};

class CST{
public:
    ~CST();
    CST(const std::vector<Token*>& tokens, const std::string& parseTable);
private:
    CSTNode* root;
};


#endif //TRANSPILER_CSTNODE_H
