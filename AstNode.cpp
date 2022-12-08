//
// Created by stein on 29/11/2022.
//

#include "AstNode.h"

AstNode::AstNode(Token *token): token(token) {}

AstNode::AstNode() {}

std::string AstNode::getTokenType() {
    return token->getType();
}

std::string AstNode::getTokenText() {
    return token->getText();
}

bool AstNode::checkTypes(SymbolTable &table) {
    return true;
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

AstDeclartion::AstDeclartion(Token *token) : AstNode(token) {

}

AstDeclartion::AstDeclartion(Token *token, AstVar *var, AstValue *value): AstNode(token), var(var), value(value) {

}

void AstDeclartion::setAstVar(AstVar *var) {
    this->var = var;
}

void AstDeclartion::setAstValue(AstValue *value) {
    this->value = value;
}

AstVar *AstDeclartion::getAstVar() {
    return var;
}

AstValue *AstDeclartion::getAstValue() {
    return value;
}

std::vector<AstNode *> AstDeclartion::getChilderen() {
    return std::vector<AstNode *>{var, value};
}

std::string AstDeclartion::getJsCode() {
    return "let " + var->getTokenText() + " = " + value->getTokenText() + ";";
}

astNodeType AstDeclartion::getType() {
    return AstDeclartionC;
}

bool AstDeclartion::checkTypes(SymbolTable &table) {
    if(table.IsVarDeclared(this->var->getTokenText())){
        std::cout << "error on line: " << token->getLine() << " [" << "Variable " << this->var->getTokenText() << " is declared multiple times" << "]" << std::endl;
        return false;
    }
    table.newVar(this->var->getTokenText());
    return true;
}

AstIntalisations::AstIntalisations() {

}

AstIntalisations::AstIntalisations(AstVar *var, AstValue *value): var(var), value(value) {}

void AstIntalisations::setAstVar(AstVar *var) {
    this->var = var;
}

void AstIntalisations::setAstValue(AstValue *value) {
    this->value = value;
}

AstVar *AstIntalisations::getAstVar() {
    return var;
}

AstValue *AstIntalisations::getAstValue() {
    return value;
}

std::vector<AstNode *> AstIntalisations::getChilderen() {
    return std::vector<AstNode *>{var, value};
}

std::string AstIntalisations::getJsCode() {
    return var->getTokenText() + " = " + value->getTokenText() + ";";
}

astNodeType AstIntalisations::getType() {
    return AstIntalisationsC;
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

std::string AstIf::getJsCode() {
    return "if(" + condition->getJsCode() + ")" + body->getJsCode();
}

astNodeType AstIf::getType() {
    return AstIfC;
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
    return "(" + val1->getJsCode() + this->getTokenText() + val2->getJsCode() + ")";
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
    if(!table.IsVarInitialized(this->getTokenText())){
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
