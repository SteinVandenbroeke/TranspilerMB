//
// Created by stein on 1/12/2022.
//

#include "CSTNode.h"

CSTNode::CSTNode(Token *token) : token{token} {}

CST::CST(const std::vector<Token *> &tokens, const std::string &parseTable){
    root = new CSTNode(new Token("", ""));
    std::ifstream input(parseTable);
    json j;
    input >> j;
    std::vector<int> stack = {0};
    int index = 0;
    bool accepts = false;
    while (true){
        bool changed = false;
        Token* currToken = tokens[index];
        for (auto entry : j["ActionTable"]){
            if (entry["Input"] == currToken->getType() and entry["StateIndex"] == stack.back()){
                changed = true;
                if (entry["ActionType"] == "s"){
                    root->addChild(new CSTNode(currToken));
                    stack.push_back(entry["ActionArgument"]);
                    index++;
                }
                else if (entry["ActionType"] == "r"){
                    int productionIndex = entry["ActionArgument"];
                    for (auto production : j["Productions"]){
                        if (production["index"] == productionIndex){
                            std::vector<CSTNode*> children;
                            for (int i = 0; i < production["body"].size(); i++){
                                stack.pop_back();
                                children.push_back(root->removeChild(root->getChildren().back()));
                            }
                            for (auto gotoEntry : j["GotoTable"]){
                                if (gotoEntry["Input"] == production["head"] and gotoEntry["StateIndex"] == stack.back()){
                                    stack.push_back(gotoEntry["GotoIndex"]);
                                    break;
                                }
                            }
                            root->addChild(new CSTNode(new Token(production["head"])));
                            for (auto child : children){
                                root->getChildren().back()->addChild(child);
                            }
                        }
                    }
                }
                else if (entry["ActionType"] == "acc"){
                    accepts = true;
                }
                break; //Go to next token
            }
        }
        if (accepts){
            break;
        } else if (!changed){
            throw (std::runtime_error("LR(1) parsing error!")); //Parsing is unresolved / has failed
        }
    }
    CSTNode* temp = root;
    root = root->getChildren().back();
    for (auto child : temp->getChildren()){
        temp->removeChild(child);
    }
    delete temp;
}

CST::~CST() {
    delete root;
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

CSTNode::~CSTNode() {
    for (auto child : children){
        delete child;
    }
}
