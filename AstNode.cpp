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

bool AstNode::checkTypes(SymbolTable &table, std::stringstream& errorStream) const {
    bool typeOk = true;
    for(AstNode* item: this->getChilderen()){
        if(!item->checkTypes(table, errorStream)){
            typeOk = false;
        }
    }
    return typeOk;
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

AstNode::~AstNode() {
    for (AstNode* child : this->getChilderen()){
        delete child;
    }
}

void AstProgram::addLine(AstNode *programLine) {
    this->lines.push_back(programLine);
}

std::string AstProgram::getJsCode(int scopeCount) const {
    std::string code = "";
    std::string tabs = "";
    for(int i = 0; i < scopeCount; i++){
        tabs += "\t";
    }
    for(AstNode* programLine: lines){
        code += tabs + programLine->getJsCode(scopeCount) + "\n";
    }
    code.resize(code.size() - 1);//remove last enter
    return code;
}

bool AstProgram::checkTypes(std::stringstream& errorStream) {
    bool typesOk = true;
    SymbolTable symbolTable = SymbolTable();
    for(AstNode* programLine: lines){
        if(!programLine->checkTypes(symbolTable, errorStream)){
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

AstProgram::~AstProgram() {
    for (AstNode* child : this->lines){
        delete child;
    }
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

std::string AstDeclartion::getJsCode(int scopeCount) const {
    return "let " + var->getTokenText() + " = " + value->getJsCode(scopeCount) + ";";
}

bool AstDeclartion::checkTypes(SymbolTable &table, std::stringstream& errorStream) const {
    if(table.IsVarInCurrentScope(this->var->getTokenText())){
        errorStream << "error on line: " << token->getLine() << " [" << "Variable " << this->var->getTokenText() << " is declared multiple times in the same scope" << "]" << std::endl;
        return false;
    }
    table.newVar(this->var->getTokenText(), this->getTokenText());
    if(!table.IsAllowedType(this->var->getType(table), this->value->getType(table))){
        errorStream << "error on line: " << token->getLine() << " [" << "Variable " << this->var->getTokenText() << " of type \"" + this->var->getType(table) + "\" does not accept a value of type \"" + this->value->getType(table)  << "\"]" << std::endl;
        return false;
    }
    return true;
}

AstDeclartion::~AstDeclartion() {
    delete var;
    delete value;
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

std::string AstIntalisation::getJsCode(int scopeCount) const {
    return var->getTokenText() + " = " + value->getJsCode(scopeCount) + ";";
}

bool AstIntalisation::checkTypes(SymbolTable &table, std::stringstream& errorStream) const {
    if(!table.IsVarDeclared(this->var->getTokenText())){
        errorStream << "error on line: " << token->getLine() << " [" << "Variable " << this->var->getTokenText() << " is not declared" << "]" << std::endl;
        return false;
    }
    else if(!table.IsAllowedType(this->var->getType(table), this->value->getType(table))){
        errorStream << "error on line: " << token->getLine() << " [" << "Variable " << this->var->getTokenText() << " of type \"" + this->var->getType(table) + "\" does not accept a value of type \"" + this->value->getType(table)  << "\"]" << std::endl;
        return false;
    }
    return true;
}

AstIntalisation::~AstIntalisation() {
    delete var;
    delete value;
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

AstConditionBody::~AstConditionBody() {
    delete condition;
    delete body;
}

std::string AstWhile::getJsCode(int scopeCount) const {
    return "while" + condition->getJsCode(scopeCount) + "" + body->getJsCode(scopeCount);
}

AstWhile::AstWhile(Token* token,AstCondition *condition, AstBody *body) : AstConditionBody(token,condition, body) {}

std::string AstWhile::getValue() const {
    return "while";
}

std::string AstIf::getJsCode(int scopeCount) const {
    return "if" + condition->getJsCode(scopeCount) + "" + body->getJsCode(scopeCount);
}

AstIf::AstIf(Token* token,AstCondition *condition, AstBody *body) : AstConditionBody(token,condition, body) {}

std::string AstBody::getJsCode(int scopeCount) const {
    std::string tabs = "";
    for(int i = 0; i < scopeCount; i++){
        tabs += "\t";
    }
    return "{\n" + AstProgram::getJsCode(scopeCount + 1) + "\n" + tabs + "}";
}

bool AstBody::checkTypes(SymbolTable &table, std::stringstream& errorStream) const {
    bool typesOk = true;
    SymbolTable* scopeTable = table.newScope();
    SymbolTable &table1 = *scopeTable;
    for(AstNode* programLine: this->lines){
        if(!programLine->checkTypes(table1, errorStream)){
            typesOk = false;
        }
    }
    scopeTable->removeScope();
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

std::string AstCondition::getJsCode(int scopeCount) const {
    return "(" + val1->getJsCode(scopeCount) + " " + this->getTokenText() + " " + val2->getJsCode(scopeCount) + ")";
}

bool AstCondition::checkTypes(SymbolTable &table, std::stringstream& errorStream) const {
    if(val1->getType(table) != val2->getType(table)){
        errorStream << "error on line: " << token->getLine() << " [" << "Unable to compare type: \"" << this->val1->getType(table) << " to type " + this->val2->getType(table)  << "\"]" << std::endl;
        return false;
    }
    return true;
}

AstCondition::~AstCondition() {
    delete val1;
    delete val2;
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

std::string AstArithmeticOperations::getJsCode(int scopeCount) const {
    return val1->getJsCode(scopeCount) + this->getTokenText() + val2->getJsCode(scopeCount);
}

std::string AstArithmeticOperations::getType(SymbolTable &table) const {
    if((val1->getType(table) == "int" && val2->getType(table) == "double") || (val1->getType(table) == "double" && val2->getType(table) == "int") || (val1->getType(table) == "double" && val2->getType(table) == "double")){
        return "double";
    }
    else if(val1->getType(table) == "int" && val2->getType(table) == "int"){
        return "int";
    }
    else if(this->getTokenText() == "+" && val1->getType(table) != "undefined" && val2->getType(table) != "undefined"){
        return "string";
    }
    return "undefined";
}

bool AstArithmeticOperations::checkTypes(SymbolTable &table, std::stringstream& errorStream) const {
    if(!table.IsAllowedType(val1->getType(table), val2->getType(table)) && !table.IsAllowedType(val2->getType(table), val1->getType(table))){
        errorStream << "error on line: " << token->getLine() << " [" << "Arithmetic operations between two types not allowed: \"" << this->val1->getType(table) << "\" " + this->getTokenText() + " \"" + this->val2->getType(table)  << "\"]" << std::endl;
        return false;
    }
    return true;
}

AstArithmeticOperations::~AstArithmeticOperations() {
    delete val1;
    delete val2;
}

AstVarOrValue::AstVarOrValue(Token *token) : AstNode(token) {}

AstVar::AstVar(Token *token) : AstVarOrValue(token) {}

std::string AstVar::getJsCode(int scopeCount) const {
    return token->getText();
}

std::string AstVar::getType(SymbolTable& table) const {
    return table.getVarType(this->token->getText());
}

bool AstVar::checkTypes(SymbolTable &table, std::stringstream& errorStream) const {
    if(!table.IsVarDeclared(this->getTokenText())){
        errorStream << "error on line: " << token->getLine() << " [" << "Variable " << this->getTokenText() << " is not declared." << "]" << std::endl;
        return false;
    }
    return true;
}

AstValue::AstValue(Token *token) : AstVarOrValue(token) {}

std::string AstValue::getJsCode(int scopeCount) const {
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

std::string AstParentheses::getJsCode(int scopeCount) const {
    return "(" + this->innerNode->getJsCode(scopeCount) + ")";
}

std::string AstParentheses::getType(SymbolTable& table) const {
    return this->innerNode->getType(table);
}

std::string AstParentheses::getValue() const {
    return "()";
}

AstParentheses::~AstParentheses() {
    delete innerNode;
}

AstPrint::AstPrint(Token *token) : AstNode(token) {

}

AstPrint::AstPrint(Token *token, AstNode *value): AstNode(token), value(value) {

}

std::vector<AstNode *> AstPrint::getChilderen() const {
    return std::vector<AstNode *>{value};
}

std::string AstPrint::getJsCode(int scopeCount) const {
    return "console.log(" + this->value->getJsCode(scopeCount) + ");";
}

AstPrint::~AstPrint() {
    delete value;
}
