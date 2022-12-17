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

std::string AstNode::getTokenType() const {
    return token->getType();
}

std::string AstNode::getTokenText() const {
    return token->getText();
}

bool AstNode::checkTypes(SymbolTable &table) const {
    bool typeOk = true;
    for(AstNode* item: this->getChilderen()){
        if(!item->checkTypes(table)){
            typeOk = false;
        }
    }
    return false;
}

std::vector<AstNode *> AstNode::getChilderen() const {
    return std::vector<AstNode *>{};
}

std::string AstNode::generateDotInternal() const {
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

std::string AstNode::generateDOT() const {
    std::string dot  = "digraph G {\n";
    dot += this->generateDotInternal();
    dot += "\n}";
    return dot;
}

std::string AstNode::getValue() const {
    return this->token->getText();
}

std::string AstNode::getType(SymbolTable &table) const {
    return "identifier";
}

void AstProgram::addLine(AstNode *programLine) {
    this->lines.push_back(programLine);
}

std::string AstProgram::getJsCode() const {
    std::string code = "";
    for(AstNode* programLine: lines){
        code += programLine->getJsCode() + "\n";
    }
    return code;
}

bool AstProgram::checkTypes() {
    bool typesOk = true;
    SymbolTable symbolTable = SymbolTable();
    for(AstNode* programLine: lines){
        if(!programLine->checkTypes(symbolTable)){
            typesOk = false;
        }
    }
    return typesOk;
}

std::vector<AstNode *> AstProgram::getChilderen() const {
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

std::vector<AstNode *> AstDeclartion::getChilderen() const {
    return std::vector<AstNode *>{var, value};
}

std::string AstDeclartion::getJsCode() const {
    return "let " + var->getTokenText() + " = " + value->getJsCode() + ";";
}

bool AstDeclartion::checkTypes(SymbolTable &table) const {
    if(table.IsVarDeclared(this->var->getTokenText())){
        std::cout << "error on line: " << token->getLine() << " [" << "Variable " << this->var->getTokenText() << " is declared multiple times in the same scope" << "]" << std::endl;
        return false;
    }
    table.newVar(this->var->getTokenText(), this->getTokenText());
    if(!table.IsAllowedType(this->var->getTokenText(), this->value->getType(table))){
        std::cout << "error on line: " << token->getLine() << " [" << "Variable " << this->var->getTokenText() << " of type \"" + this->var->getType(table) + "\" does not accept a value of type \"" + this->value->getType(table)  << "\"]" << std::endl;
        return false;
    }
    return true;
}

AstIntalisation::AstIntalisation(Token* token): AstNode(token) {

}

AstIntalisation::AstIntalisation(Token* token, AstVar *var, AstNode *value): AstNode(token), var(var), value(value) {}

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

std::vector<AstNode *> AstIntalisation::getChilderen() const {
    return std::vector<AstNode *>{var, value};
}

std::string AstIntalisation::getJsCode() const {
    return var->getTokenText() + " = " + value->getJsCode() + ";";
}

bool AstIntalisation::checkTypes(SymbolTable &table) const {
    if(!table.IsVarDeclared(this->var->getTokenText())){
        std::cout << "error on line: " << token->getLine() << " [" << "Variable " << this->var->getTokenText() << " is not declared" << "]" << std::endl;
        return false;
    }
    else if(!table.IsAllowedType(this->var->getTokenText(), this->value->getType(table))){
        std::cout << "error on line: " << token->getLine() << " [" << "Variable " << this->var->getTokenText() << " of type \"" + this->var->getType(table) + "\" does not accept a value of type \"" + this->value->getType(table)  << "\"]" << std::endl;
        return false;
    }
    return true;
}

AstConditionBody::AstConditionBody(Token* token): AstNode(token){

}

AstConditionBody::AstConditionBody(Token* token, AstCondition *condition, AstBody *body): AstNode(token), condition(condition), body(body) {}

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

std::vector<AstNode *> AstConditionBody::getChilderen() const {
    return std::vector<AstNode *>{condition, body};
}

std::string AstWhile::getJsCode() const {
    return "while" + condition->getJsCode() + "" + body->getJsCode();
}

AstWhile::AstWhile(Token* token,AstCondition *condition, AstBody *body) : AstConditionBody(token,condition, body) {}

std::string AstWhile::getValue() const {
    return "while";
}

std::string AstIf::getJsCode() const {
    return "if" + condition->getJsCode() + "" + body->getJsCode();
}

AstIf::AstIf(Token* token,AstCondition *condition, AstBody *body) : AstConditionBody(token,condition, body) {}

std::string AstBody::getJsCode() const {
    return "{\n" + AstProgram::getJsCode() + "\n}";
}

bool AstBody::checkTypes(SymbolTable &table) const {
    bool typesOk = true;
    table.newScope();
    for(AstNode* programLine: this->lines){
        if(!programLine->checkTypes(table)){
            typesOk = false;
        }
    }
    return typesOk;
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

std::vector<AstNode *> AstCondition::getChilderen() const {
    return std::vector<AstNode *>{val1, val2};
}

std::string AstCondition::getJsCode() const {
    return "(" + val1->getJsCode() + this->getTokenText() + val2->getJsCode() + ")";
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

std::vector<AstNode *> AstArithmeticOperations::getChilderen() const {
    return std::vector<AstNode *>{val1, val2};
}

std::string AstArithmeticOperations::getJsCode() const {
    return val1->getJsCode() + this->getTokenText() + val2->getJsCode();
}

std::string AstArithmeticOperations::getType(SymbolTable &table) const {
    if((val1->getType(table) == "int" && val2->getType(table) == "double") || (val1->getType(table) == "double" && val2->getType(table) == "int") || (val1->getType(table) == "double" && val2->getType(table) == "double")){
        return "double";
    }
    else if(val1->getType(table) == "int" && val2->getType(table) == "int"){
        return "int";
    }
    else if((val1->getType(table) == "string" && val2->getType(table) == "char") || (val1->getType(table) == "char" && val2->getType(table) == "string")){
        return "string";
    }
    return "undefined";
}

AstVarOrValue::AstVarOrValue(Token *token) : AstNode(token) {}

AstVar::AstVar(Token *token) : AstVarOrValue(token) {}

std::string AstVar::getJsCode() const {
    return token->getText();
}

std::string AstVar::getType(SymbolTable& table) const {
    return table.getVarType(this->token->getText());
}

bool AstVar::checkTypes(SymbolTable &table) const {
    if(!table.IsVarDeclared(this->getTokenText())){
        std::cout << "error on line: " << token->getLine() << " [" << "Variable " << this->getTokenText() << " is not declared." << "]" << std::endl;
        return false;
    }
    return true;
}

AstValue::AstValue(Token *token) : AstVarOrValue(token) {}

std::string AstValue::getJsCode() const {
    return token->getText();
}

std::string AstValue::getType(SymbolTable& table) const {
    std::string value = this->getTokenText();
    if(value[0] == '"' && value[this->getTokenText().size() - 1] == '"'){
        return "string";
    }
    else if(value[0] == '\'' && value[2] == '\'' && value.size() == 3){
        return "char";
    }
    else if(value.find('.') != std::string::npos){
        return "double";
    }
    return "int";
}

AstParentheses::AstParentheses(Token *token, AstNode *innerNode): AstNode(token), innerNode(innerNode) {

}

std::vector<AstNode *> AstParentheses::getChilderen() const {
    return std::vector<AstNode *>{innerNode};
}

std::string AstParentheses::getJsCode() const {
    return "(" + this->innerNode->getJsCode() + ")";
}

std::string AstParentheses::getType(SymbolTable& table) const {
    return this->innerNode->getType(table);
}

std::string AstParentheses::getValue() const {
    return "()";
}

AstPrint::AstPrint(Token *token) : AstNode(token) {

}

AstPrint::AstPrint(Token *token, AstNode *value): AstNode(token), value(value) {

}

std::vector<AstNode *> AstPrint::getChilderen() const {
    return std::vector<AstNode *>{value};
}

std::string AstPrint::getJsCode() const {
    return "console.log(" + this->value->getJsCode() + ");";
}