//
// Created by stein on 1/12/2022.
//

#include "CSTNode.h"

CSTNode::CSTNode(Token *token) : token{token} {}

CST::CST(std::vector<Token *> &tokens, const std::string &parseTable) {
    std::ifstream input(parseTable);
    json j;
    input >> j;
    std::vector<int> stack = {0};
    int index = 0;
    std::vector<std::string> terminals = j["Terminals"];
    std::vector<std::string> variables = j["Variables"];
    while (true){
        bool hasAccepted = false;
        Token* currToken = tokens[index];
        for (const auto& variable : variables){
            if (currToken->getText() == variable){
                index++;
                continue;
            }
        }
        for (auto entry : j["ActionTable"]){
            if (entry["Input"] == currToken->getText() and entry["StateIndex"] == stack.back()){
                if (entry["ActionType"] == "s"){
                    root->addChild(new CSTNode(currToken));
                    stack.push_back(entry["ActionArgument"]);
                    index++;
                }
                else if (entry["ActionType"] == "r"){

                } else{
                    hasAccepted = true;
                }
                break;
            }
        }
        if (hasAccepted){
            break;
        }
    }
}

const std::vector<CSTNode *> &CSTNode::getChildren() const {
    return children;
}

Token *CSTNode::getToken() const {
    return token;
}

CSTNode *CSTNode::removeChild(CSTNode *node) {
    for (auto it = children.begin(); it != children.end(); it++){
        if (*it == node){
            CSTNode* temp = *it;
            children.erase(it);
            return temp;
        }
    }
    return nullptr;
}

void CSTNode::addChild(CSTNode *node) {
    children.push_back(node);
}
