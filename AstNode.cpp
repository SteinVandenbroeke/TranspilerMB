//
// Created by stein on 29/11/2022.
//

#include "AstNode.h"

AstNode::AstNode(Token *token): token(token) {
    this->identifier = identifierCounter;
    identifierCounter++;
}

AstNode::AstNode() {
    this->identifier = identifierCounter;
    identifierCounter++;
}

std::string AstNode::getTokenType() {
    return token->getType();
}

std::string AstNode::getTokenText() {
    return token->getText();
}

bool AstNode::checkTypes(SymbolTable &table) {
    for(AstNode* item: this->getChilderen()){
        item->checkTypes(table);
    }
    return true;
}

std::vector<AstNode *> AstNode::getChilderen() {
    return std::vector<AstNode *>{};
}

std::string AstNode::generateDotInternal() {
    std::string dot = "";

    std::string value = this->getValue();
    if(value[0] == '"' && value[value.length() - 1] == '"'){
        value.erase(value.begin());
        value.pop_back();
        value = "\\\"" + value + "\\\"";
    }
    dot += "\"" + this->getIdentifier() + "\"" + " [label=\"" + value + "\"];\n";
    for (auto child : this->getChilderen()){
        dot += child->generateDotInternal();
        dot += "\"" + this->getIdentifier() + "\"" + " -> " +  "\"" + child->getIdentifier() + "\"" + "\n";
    }
    return dot;
}

std::string AstNode::getIdentifier() const {
    std::string objectName = typeid(this).name();
    return objectName + " [" + std::to_string(this->identifier) + "]";
}

std::string AstNode::generateDOT() {
    std::string dot  = "digraph G {\n";
    dot += this->generateDotInternal();
    dot += "\n}";
    return dot;
}

std::string AstNode::getValue() const {
    return this->token->getText();
}

void AstProgram::addLine(AstNode *programLine) {
    this->lines.push_back(programLine);
}

std::string AstProgram::getJsCode() {
    std::string code = "";
    for(AstNode* programLine: lines){
        code += programLine->getJsCode() + "\n";
    }
    return code;
}

astNodeType AstProgram::getType() {
    return AstProgramC;
}

bool AstProgram::checkTypes() {
    SymbolTable symbolTable = SymbolTable();
    for(AstNode* programLine: lines){
        programLine->checkTypes(symbolTable);
    }
    return true;
}

std::vector<AstNode *> AstProgram::getChilderen() {
    return this->lines;
}

std::string AstProgram::getValue() const {
    return "Program";
}

AstDeclartion::AstDeclartion(Token *token) : AstNode(token) {

}

AstDeclartion::AstDeclartion(Token *token, AstVar *var, AstNode *value): AstNode(token), var(var), value(value) {

}

void AstDeclartion::setAstVar(AstVar *var) {
    this->var = var;
}

void AstDeclartion::setAstValue(AstNode *value) {
    this->value = value;
}

AstVar *AstDeclartion::getAstVar() {
    return var;
}

AstNode *AstDeclartion::getAstValue() {
    return value;
}

std::vector<AstNode *> AstDeclartion::getChilderen() {
    return std::vector<AstNode *>{var, value};
}

std::string AstDeclartion::getJsCode() {
    return "let " + var->getTokenText() + " = " + value->getJsCode() + ";";
}

astNodeType AstDeclartion::getType() {
    return AstDeclartionC;
}

bool AstDeclartion::checkTypes(SymbolTable &table) {
    if(table.IsVarDeclared(this->var->getTokenText(), this->getTokenText())){
        std::cout << "error on line: " << token->getLine() << " [" << "Variable " << this->var->getTokenText() << " is declared multiple times" << "]" << std::endl;
        return false;
    }
    table.newVar(this->var->getTokenText(), this->getTokenText());
    return true;
}

AstIntalisation::AstIntalisation() {

}

AstIntalisation::AstIntalisation(AstVar *var, AstNode *value): var(var), value(value) {}

void AstIntalisation::setAstVar(AstVar *var) {
    this->var = var;
}

void AstIntalisation::setAstValue(AstValue *value) {
    this->value = value;
}

AstVar *AstIntalisation::getAstVar() {
    return var;
}

AstNode *AstIntalisation::getAstValue() {
    return value;
}

std::vector<AstNode *> AstIntalisation::getChilderen() {
    return std::vector<AstNode *>{var, value};
}

std::string AstIntalisation::getJsCode() {
    return var->getTokenText() + " = " + value->getJsCode() + ";";
}

astNodeType AstIntalisation::getType() {
    return AstIntalisationsC;
}

std::string AstIntalisation::getValue() const {
    return "=";
}

AstConditionBody::AstConditionBody() {

}

AstConditionBody::AstConditionBody(AstCondition *condition, AstBody *body): condition(condition), body(body) {}

void AstConditionBody::setAstCondition(AstCondition *condition) {
    this->condition = condition;
}

void AstConditionBody::setAstBody(AstBody *body) {
    this->body = body;
}

AstCondition *AstConditionBody::getAstCondition() {
    return condition;
}

AstBody *AstConditionBody::getAstBody() {
    return body;
}

std::vector<AstNode *> AstConditionBody::getChilderen() {
    return std::vector<AstNode *>{condition, body};
}

std::string AstWhile::getJsCode() {
    return "while(" + condition->getJsCode() + ")" + body->getJsCode();
}

astNodeType AstWhile::getType() {
    return AstWhileC;
}

AstWhile::AstWhile(AstCondition *condition, AstBody *body) : AstConditionBody(condition, body) {}

std::string AstWhile::getValue() const {
    return "while";
}

std::string AstIf::getJsCode() {
    return "if(" + condition->getJsCode() + ")" + body->getJsCode();
}

astNodeType AstIf::getType() {
    return AstIfC;
}

AstIf::AstIf(AstCondition *condition, AstBody *body) : AstConditionBody(condition, body) {}

std::string AstIf::getValue() const {
    return "if";
}

std::string AstBody::getJsCode() {
    return "{\n" + token->getText() + "\n}";
}

astNodeType AstBody::getType() {
    return AstBodyC;
}

AstCondition::AstCondition(Token *token) : AstNode(token) {

}

AstCondition::AstCondition(Token *token, AstNode *var1, AstNode *var2): AstNode(token), val1(var1), val2(var2) {

}

void AstCondition::setVar1(AstNode *valOrValue) {
    this->val1 = valOrValue;
}

void AstCondition::setVar2(AstNode *valOrValue) {
    this->val2 = valOrValue;
}

std::vector<AstNode *> AstCondition::getChilderen() {
    return std::vector<AstNode *>{val1, val2};
}

std::string AstCondition::getJsCode() {
    return "(" + val1->getJsCode() + this->getTokenText() + val2->getJsCode() + ")";
}

astNodeType AstCondition::getType() {
    return AstConditionC;
}

AstArithmeticOperations::AstArithmeticOperations(Token *token) : AstNode(token) {

}

AstArithmeticOperations::AstArithmeticOperations(Token *token, AstNode *val1, AstNode *val2): AstNode(token), val1(val1), val2(val2) {
    //TODO conditions on ASTNode
}

void AstArithmeticOperations::setVal1(AstNode *valOrValue) {
    //TODO conditions on ASTNode
    this->val1 = valOrValue;
}

void AstArithmeticOperations::setVal2(AstNode *valOrValue) {
    //TODO conditions on ASTNode
    this->val2 = valOrValue;
}

std::vector<AstNode *> AstArithmeticOperations::getChilderen() {
    return std::vector<AstNode *>{val1, val2};
}

std::string AstArithmeticOperations::getJsCode() {
    return val1->getJsCode() + this->getTokenText() + val2->getJsCode();
}

astNodeType AstArithmeticOperations::getType() {
    return AstArithmeticOperationsC;
}

AstVarOrValue::AstVarOrValue(Token *token) : AstNode(token) {}

AstVar::AstVar(Token *token) : AstVarOrValue(token) {}

std::string AstVar::getJsCode() {
    return token->getText();
}

astNodeType AstVar::getType() {
    return AstVarC;
}

bool AstVar::checkTypes(SymbolTable &table) {
    if(!table.IsVarDeclared(this->getTokenText(), "/")){
        std::cout << "error on line: " << token->getLine() << " [" << "Variable " << this->getTokenText() << " is not initialized." << "]" << std::endl;
        return false;
    }
    return true;
}

AstValue::AstValue(Token *token) : AstVarOrValue(token) {}

std::string AstValue::getJsCode() {
    return token->getText();
}

astNodeType AstValue::getType() {
    return AstValueC;
}

AstParentheses::AstParentheses(Token *token, AstNode *innerNode): AstNode(token), innerNode(innerNode) {

}

std::vector<AstNode *> AstParentheses::getChilderen() {
    return std::vector<AstNode *>{innerNode};
}

std::string AstParentheses::getJsCode() {
    return "(" + this->innerNode->getJsCode() + ")";
}

astNodeType AstParentheses::getType() {
    return AstParenthesesC;
}

std::string AstParentheses::getValue() const {
    return "()";
}
