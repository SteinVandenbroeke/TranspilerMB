//
// Created by stein on 1/12/2022.
//

#ifndef TRANSPILER_CSTNODE_H
#define TRANSPILER_CSTNODE_H
#include "tokenizer/Token.h"
#include <vector>
#include "json.hpp"
#include "AstNode.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>
using nlohmann::json;

class CSTNode {
public:
    virtual ~CSTNode();
    CSTNode();
    virtual std::string getValue() const = 0;
    virtual Token* getToken() const;
    const std::vector<CSTNode *> &getChildren() const;
    CSTNode* removeChild(CSTNode* node);
    void addChild(CSTNode* node);
    int getIdentifier() const;
    virtual AstNode* toAst(AstProgram* program) const;
private:
    int identifier;
    std::vector<CSTNode*> children;
};

class LeafNode : public CSTNode {
public:
    LeafNode(Token* token);
    virtual std::string getValue() const override;
    Token *getToken() const override;
    AstNode* toAst(AstProgram* program = nullptr) const override;
private:
    Token* token;
};

class InternalNode : public CSTNode{
public:
    virtual std::string getValue() const override;
    InternalNode(const std::string& value);
private:
    std::string value;
};

class CST{
public:
    void DOTNode(std::string& s, CSTNode* node) const;
    std::string generateDOT() const;
    void assignNodes(std::string& s, CSTNode* node) const;
    ~CST();
    CST(const std::vector<Token*>& tokens, const std::string& parseTable);
    AstProgram* toAst() const;
private:
    CSTNode* root;
};


#endif //TRANSPILER_CSTNODE_H
