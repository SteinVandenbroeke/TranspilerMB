//
// Created by stein on 7/12/2022.
//

#include <iostream>
#include "SymbolTable.h"

SymbolTable::SymbolTable() {

}

SymbolTable::SymbolTable(SymbolTable *outerScope) {
    this->outerScope = outerScope;
}

SymbolTable *SymbolTable::newScope() {
    return new SymbolTable(this);
}

SymbolTable *SymbolTable::removeScope() {
    SymbolTable* outerScope = this->outerScope;
    delete this;
    return outerScope;
}

bool SymbolTable::IsVarInCurrentScope(std::string varName, std::string varType) {
    if(table.find(varName) != table.end() && table[varName] == varType){
        return true;
    }
    return false;
}

bool SymbolTable::IsVarDeclared(std::string varName, std::string varType) {
    if(table.find(varName) != table.end() && table[varName] == varType){
        return true;
    }
    else if(outerScope == nullptr){
        return false;
    }
    return outerScope->IsVarDeclared(varName, varType);
}


void SymbolTable::newVar(std::string varName, std::string varType) {
    this->table[varName] = varType;
}

bool SymbolTable::IsVarDeclared(std::string varName) {
    if(table.find(varName) != table.end()){
        return true;
    }
    else if(outerScope == nullptr){
        return false;
    }
    return outerScope->IsVarDeclared(varName);
}

std::string SymbolTable::getVarType(std::string varName) {
    if(table.find(varName) != table.end()){
        return table[varName];
    }
    else if(outerScope == nullptr){
        return "none";
    }
    return outerScope->getVarType(varName);
}

bool SymbolTable::IsAllowedType(const std::string& compaireType1, const std::string& compaireType2, std::stringstream& errorStream) const {
    if((compaireType1 == compaireType2) || (compaireType1 == "double" && compaireType2 == "int") || (compaireType1 == "string" && compaireType2 == "char") || (compaireType1 == "string" && compaireType2 == "int") || (compaireType1 == "string" && compaireType2 == "double") || (compaireType1 == "double" && compaireType2 == "string")){
        return true;
    }
    else if(compaireType1 == "int" && compaireType2 == "double"){
        errorStream << "[Warning] implicit type conversion found: double to int" << std::endl;
        return true;
    }
    return false;
}

bool SymbolTable::IsVarInCurrentScope(std::string varName) {
    return table.find(varName) != table.end();
}
