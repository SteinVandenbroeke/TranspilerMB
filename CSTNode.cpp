//
// Created by stein on 1/12/2022.
//

#include "CSTNode.h"

LeafNode::LeafNode(Token *token) : token{token} {}

CST::CST(const std::vector<Token *> &tokens, const std::string &parseTable){
    root = new InternalNode("");
    std::ifstream input(parseTable);
    json j;
    input >> j;
    std::vector<int> stack = {0};
    std::map<std::pair<int, std::string>, std::pair<int, std::string>> actionTable;
    std::map<std::pair<int, std::string>, int> gotoTable;
    std::vector<std::pair<std::string, std::string>> productions(j["NumberOfProductions"], {"", ""});
    for (auto entry : j["ActionTable"]){
        int actionArgument;
        if (entry["ActionType"] != "acc"){
            actionArgument = entry["ActionArgument"];
        } else{
            actionArgument = -1;
        }
        actionTable[{entry["StateIndex"], entry["Input"]}] = {actionArgument, entry["ActionType"]};
    }
    for (auto entry : j["GotoTable"]){
        gotoTable[{entry["StateIndex"], entry["Input"]}] = entry["GotoIndex"];
    }
    for (auto entry : j["Productions"]){
        std::string rightSide;
        for (const auto& character : entry["body"]){
            rightSide += character;
        }
        productions[entry["index"]] = {entry["head"], rightSide};
    }
    int index = 0;
    while (true){
        Token* currToken = tokens[index];
        if (actionTable.find({stack.back(), currToken->getType()}) != actionTable.end()){
            std::pair<int, std::string> actionPair = actionTable[{stack.back(), currToken->getType()}];
            if (actionPair.second == "s"){
                root->addChild(new LeafNode(currToken));
                stack.push_back(actionPair.first);
                index++;
            }
            else if (actionPair.second == "r"){
                std::vector<CSTNode*> children;
                for (int i = 0; i < productions[actionPair.first].second.size(); i++){
                    stack.pop_back();
                    children.push_back(root->removeChild(root->getChildren().back()));
                }
                stack.push_back(gotoTable[{stack.back(), productions[actionPair.first].first}]);
                root->addChild(new InternalNode(productions[actionPair.first].first));
                for (auto child : children){
                    root->getChildren().back()->addChild(child);
                }
            }
            else if (actionPair.second == "acc"){
                break;
            }
        } else {
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

Token *LeafNode::getToken() const {
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

const std::string &InternalNode::getValue() const {
    return value;
}

InternalNode::InternalNode(const std::string& value) : value{value}{

}
