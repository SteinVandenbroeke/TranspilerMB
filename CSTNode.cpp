//
// Created by stein on 1/12/2022.
//

#include "CSTNode.h"

int id = 0;

LeafNode::LeafNode(Token *token) : token{token} {}

CST::CST(const std::vector<Token *> &tokens, const std::string &parseTable){
    root = new InternalNode("");
    std::ifstream input(parseTable);
    json j;
    input >> j;
    std::vector<int> stack = {0};
    std::map<std::pair<int, std::string>, std::pair<int, std::string>> actionTable;
    std::map<std::pair<int, std::string>, int> gotoTable;
    std::vector<std::pair<std::string, std::vector<std::string>>> productions(j["NumberOfProductions"], {"", {""}});
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
        productions[entry["index"]] = {entry["head"], entry["body"]};
    }
    int index = 0;
    while (true){
        Token* currToken = tokens[index];
        if (index == 15){
            std::cout << "";
        }
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
                    //children.push_back(root->removeChild(root->getChildren().back()));TODO Stein heeft hier aangepast
                    children.insert(children.begin(), root->removeChild(root->getChildren().back()));
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
            throw (std::runtime_error("LR(1) parsing error! on line " + std::to_string(currToken->getLine()))); //Parsing is unresolved / has failed
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

std::string CST::generateDOT() const {
    std::string DOT = "digraph G {\n";
    assignNodes(DOT, root);
    DOTNode(DOT, root);
    DOT += "}";
    return DOT;
}

void CST::DOTNode(std::string &s, CSTNode* node) const {
    for (auto i : node->getChildren()){
        s += std::to_string(node->getIdentifier()) + " -> " + std::to_string(i->getIdentifier()) + "\n";
    }
    for (auto i : node->getChildren()){
        DOTNode(s, i);
    }
}

void CST::assignNodes(std::string &s, CSTNode *node) const {
    std::string value = node->getValue();
    if(value[0] == '"' && value[value.length() - 1] == '"'){
        value.erase(value.begin());
        value.pop_back();
        value = "\\\"" + value + "\\\"";
    }
    s += std::to_string(node->getIdentifier()) + " [label=\"" + value + "\"];\n";
    for (auto i : node->getChildren()){
        assignNodes(s, i);
    }
}

AstProgram* CST::toAst() const {
    AstProgram* program = new AstProgram();
    root->toAst(program);
    return program;
}

const std::vector<CSTNode *> &CSTNode::getChildren() const {
    return children;
}

Token *LeafNode::getToken() const {
    return token;
}

std::string LeafNode::getValue() const {
    return token->getText();
}

AstNode *LeafNode::toAst(AstProgram *program) const {
    if(token->getType() == "[VARNAMETOKEN]"){
        return new AstVar(this->token);
    }
    else if(token->getType() == "[NUMBERTOKEN]" || token->getType() == "[STRINGTOKEN]" || token->getType() == "[CHARTOKEN]"){
        return new AstValue(this->token);
    }
    return nullptr;
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

int CSTNode::getIdentifier() const {
    return CSTNode::identifier;
}

CSTNode::CSTNode() : identifier(id){
    id++;
}

AstNode *CSTNode::toAst(AstProgram *program) const {
    if(this->getValue() == "ProgramLine"){
        program->addLine(this->getChildren()[0]->toAst(program));
        return nullptr;
    }
    else if(this->getValue() == "S"){
        this->getChildren()[0]->toAst(program);
        if(this->getChildren().size() > 1) {
            this->getChildren()[1]->toAst(program);
        }
        return program;
    }
    else if(this->getValue() == "VarName" || this->getValue() == "numberI" || this->getValue() == "stringI" || this->getValue() == "charI" || this->getValue() == "AnyType"){
        return this->getChildren()[0]->toAst(program);
    }
    else if(this->getValue() == "Declatation"){
        AstVar* astVar = dynamic_cast<AstVar *>(this->children[1]->toAst(program));
        AstNode* astValue = this->children[3]->toAst(program);
        return new AstDeclartion(this->children[0]->getToken(), astVar, astValue);
    }
    else if(this->getValue() == "Initalization"){
        AstVar* astVar = dynamic_cast<AstVar *>(this->children[0]->toAst(program));
        AstNode* astValue = this->children[2]->toAst(program);
        return new AstIntalisation(this->getChildren()[1]->getToken(), astVar, astValue);
    }
    else if(this->getValue() == "ArithmeticOperations" || this->getValue() == "ArithmeticOperations1"){
        if(this->children.size() == 1){
            return this->children[0]->toAst(program);//dynamic_cast<AstValue *>(this->children[0]->getChildren()[0]->toAst());
        }
        else if(this->children.size() == 3 && this->children[0]->getValue() != "(" && this->children[2]->getValue() != ")"){
            return new AstArithmeticOperations(this->children[1]->getToken(), this->children[0]->toAst(program), this->children[2]->toAst(program));
        }
        else if(this->children.size() == 3 && this->children[0]->getValue() == "(" && this->children[2]->getValue() == ")"){
            return new AstParentheses(this->children[0]->getToken(), this->children[1]->toAst(program));
        }
        else{
            throw (std::runtime_error("Wrong ArithmeticOperations node format"));
        }
    }
    else if(this->getValue() == "Condition"){
        return new AstCondition(this->children[1]->getChildren()[0]->getToken(), this->children[0]->toAst(program), this->children[2]->toAst(program));
    }
    else if(this->getValue() == "If"){
        AstBody* astBody = new AstBody();
        this->children[4]->getChildren()[1]->toAst(astBody);
        AstCondition* astCondition = dynamic_cast<AstCondition *>(this->children[2]->toAst(program));
        return new AstIf(this->children[0]->getToken(), astCondition, astBody);
    }
    else if(this->getValue() == "Loop"){
        AstBody* astBody = new AstBody();
        this->children[4]->getChildren()[1]->toAst(astBody);
        AstCondition* astCondition = dynamic_cast<AstCondition *>(this->children[2]->toAst(program));
        return new AstWhile(this->children[0]->getToken(), astCondition, astBody);
    }
    else if(this->getValue() == "Print"){
        AstNode* astValue = this->children[2]->toAst(program);
        return new AstPrint(this->children[0]->getToken(), astValue);
    }
    return nullptr;
}

Token *CSTNode::getToken() const {
    return nullptr;
}

InternalNode::InternalNode(const std::string& value) : value{value} {}

std::string InternalNode::getValue() const {
    return value;
}

